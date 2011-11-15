#! /bin/csh -f

#------------------------------------------------------------------
# Perform long term archiving of model output from short term archive 
# directories to mass store.
# Can be run interactively or submitted as a single pe batch job.
# Can optionally copy subset of files to another site.  Subset of files
#   are hardwired below.
# Long term archiver writes files to mass store on first pass, checks 
#   that file made it to the mass store fine on next submission of 
#   script and then removes local copy of file.  If there are any 
#   differences between local copy and remote copy, the the long term 
#   archiver writes local copy of file again.
#   It will be checked on next pass and then rewritten or removed.
# Users may want to modify
#   - the association between mass store directories and filenames
#   - the files that get scp-ed to another site
#   - the "set num" command line for other sites
#------------------------------------------------------------------

date

#------------------------------------------------------------------
# Setup trash dir and remove old stuff in trash
#------------------------------------------------------------------
set trashdir = $DOUT_S_ROOT/trash
if (! -d ${trashdir}   ) mkdir -p ${trashdir}
rm -f $trashdir/*

#------------------------------------------------------------------
# Move all files from the short term archiving directory
# Copies the files and dir structure from the short term archiving directory
# Assume there are up to two levels of dirs below $DOUT_S_ROOT
# Copies files from three levels, $DOUT_S_ROOT/$dirl1/$dirl2
#   where dirl1 is normallly [atm,lnd,ocn,ice,cpl,glc,rest]
#   and dirl2 is typically [init,hist,rest,logs]
# Has special exclusion for dirl1 != . and dirl2 == . which is
#   exactly redundant with dirl1 == . and dirl2 != . which is already done
#------------------------------------------------------------------

cd $DOUT_S_ROOT

#verify that all files will have acceptable mss path length
if (${MACH} == "bluefire") then
    set msroot_len = `echo $DOUT_L_MSROOT | wc -m`
    foreach myfile (`find . -type f`)
	set len = `echo $myfile | wc -m`
	@ tot_len = $msroot_len + $len - 3
	if ($tot_len > 128) then
	    echo "ccsm_l_archive.csh: potential mss path length for ${DOUT_S_ROOT}/${myfile} exceeds limit - exiting"
            exit -1
	endif
    end
endif

set lsmdir0 = $DOUT_L_MSROOT
$UTILROOT/Tools/ccsm_msmkdir ${lsmdir0}

foreach dirl1 (`/bin/ls -1` .)
if (-d $DOUT_S_ROOT/${dirl1}) then
  cd $DOUT_S_ROOT/${dirl1}
  if (${dirl1} != ".") then
     set lsmdir1 = ${lsmdir0}/${dirl1}
     $UTILROOT/Tools/ccsm_msmkdir ${lsmdir1}
  else
     set lsmdir1 = ${lsmdir0}
  endif

foreach dirl2 (`/bin/ls -1` . )
if (-d $DOUT_S_ROOT/${dirl1}/${dirl2} && (${dirl1} != "." || ${dirl2} == ".")) then
  cd $DOUT_S_ROOT/${dirl1}/${dirl2}
  if (${dirl2} != ".") then
     set lsmdir2 = ${lsmdir1}/${dirl2}
     $UTILROOT/Tools/ccsm_msmkdir ${lsmdir2}
  else
     set lsmdir2 = ${lsmdir1}
  endif

  set lsmdir = ${lsmdir2}

  #------------------------------------------------------------------
  # For ORNL, NERSC and NCAR machines, bundle history files into yearly 
  # tar files; bundling only occurs when a single monthly history file < 1gb 
  # in size, otherwise assume they're large enough for efficient 
  # transfer; archiving will occur when the december file is in the tar file
  # and there are no new stand-alone files laying around.
  #------------------------------------------------------------------

  #untar existing tar files without overwriting new files 
  if (-d tempdir) rm -rf tempdir
  set tarfile_list = `sh -c 'ls *.tar 2> /dev/null'`
  foreach tarfile ($tarfile_list)
    mkdir tempdir
    cp $tarfile tempdir/.
    if (${status} != 0) then
       echo "ccsm_l_archive.csh: error trying to copy existing tar file - exiting"
       exit -1
    endif

    cd tempdir
    tar -xf $tarfile
    if (${status} != 0) then
       echo "ccsm_l_archive.csh: error trying to extract existing tar file - exiting"
       exit -1
    endif

    rm $tarfile
    set file_list = `sh -c 'ls *.nc 2> /dev/null'`
    foreach file ($file_list)
       if (! -f ../${file} ) mv ${file} ../.
    end
    cd ..
    rm -rf tempdir
    rm $tarfile
  end

  #make sure tarring is done consistently throughout so that cmp will validate transfer
  if ( ${DOUT_L_HTAR} == "TRUE" && ${dirl1} != "rest") then
    set file_list = `sh -c 'ls *.h0.*-??.nc *.h.*-??.nc *.hi.*-??.nc 2> /dev/null'`
    foreach file ($file_list)
        set long_listing = `ls -l $file`
	if ( $long_listing[5] < 1000000000 ) then
	    set ytar_filename = `echo $file | sed "s/-..\.nc/\.tar/;"`
            if (-e ${ytar_filename}) then
               tar -uf ${ytar_filename} ${file}
               set stat = $status
            else
               tar -cf ${ytar_filename} ${file}
               set stat = $status
            endif
            echo "ccsm_l_archive dbug2 tar ${ytar_filename} ${file} $stat"
            if ( $stat == 0 ) then
		rm ${file}
            else
		echo "ccsm_l_archive.csh: error trying to bundle history files into yearly tar file - exiting"
		exit -1
	    endif
        endif
    end

    #pop,cice daily files
    set file_list = `sh -c 'ls *pop.h?.????-??-??.nc *cice.h?.????-??-??.nc 2> /dev/null'`
    foreach file ($file_list)
        set long_listing = `ls -l $file`
	if ( $long_listing[5] < 50000000 ) then
	    set ytar_filename = `echo $file | sed "s/-..-..\.nc/\.tar/;"`
            if (-e ${ytar_filename}) then
               tar -uf ${ytar_filename} ${file}
               set stat = $status
            else
               tar -cf ${ytar_filename} ${file}
               set stat = $status
            endif
            echo "ccsm_l_archive dbug2 tar ${ytar_filename} ${file} $stat"
            if ( $stat == 0 ) then
		rm ${file}
            else
		echo "ccsm_l_archive.csh: error trying to bundle history files into yearly tar file - exiting"
		exit -1
	    endif
        endif
    end

  endif

  #------------------------------------------------------------------
  # loop over files, try to reduce the length of ls, may fail
  #------------------------------------------------------------------
#  foreach str ( *.r* *.h0* *.h1* *.h2* *.h* *.d* *.i* )
#  /bin/ls -1 -d *$str*
#  if ($status == 0) then
#  foreach file (`/bin/ls -1 -d *$str*`)
  if (-e checkmssfile) rm -f checkmssfile
  foreach file (`/bin/ls -1`)
  if (-f $file) then

    #------------------------------------------------------------------
    # establish mass store directory name, function of relatively 
    # hardwired filenames
    #------------------------------------------------------------------
    set TO_TRASH = FALSE

# letting short-term archiver make these decisions
#    if ($file =~ *.i.*  ) set TO_TRASH = TRUE
#    if ($file =~ *.r.*  ) set TO_TRASH = TRUE
#    if ($file =~ *.r?.* ) set TO_TRASH = TRUE
#    if ($file =~ *.r??.*) set TO_TRASH = TRUE
#    if ($file =~ *.r???.*) set TO_TRASH = TRUE
#    if ($file =~ *.r????.*) set TO_TRASH = TRUE
#    if (${dirl1} =~ rest*) set TO_TRASH = FALSE

    #------------------------------------------------------------------
    # try to copy file from mass store, into local name checkmssfile
    # compare local file name and remote file, either remove or write to
    # mass store based on cmp return status
    # at ORNL and NCAR, compare merely by file size!! - temporarily disabled,
    # see bug #700 for details 
    #------------------------------------------------------------------
#    if ( $SITE == "ORNL" || $SITE == "NCAR") then
#	set listing = `ls -l $file`
#	$UTILROOT/Tools/ccsm_msls ${lsmdir}/$file >& tempfile
#	set lt_listing = `cat tempfile | tail -1`
#	rm -f tempfile
#	set cmp_status = 1
#	if ( ${#lt_listing} >= 5 ) then
#	    if ( $listing[5] == $lt_listing[5] ) then
#		set cmp_status = 0
#	    endif
#	endif
#    else
	$UTILROOT/Tools/ccsm_msread ${lsmdir}/$file checkmssfile
        if (-e checkmssfile) then
	    cmp -s $file checkmssfile
	    set cmp_status = $status
	else
	    set cmp_status = 99
        endif
#    endif

    if ($cmp_status == 0) then
      #------------------------------------------------------------------
      # remove file
      #------------------------------------------------------------------
      echo rm $file
      rm -f $file
    else
      #------------------------------------------------------------------
      # write file to trash or mass store
      #------------------------------------------------------------------
      if (${TO_TRASH} == TRUE) then
         echo mv $file $trashdir
         mv $file $trashdir
      else
	 if ($file =~ *.tar && -e checkmssfile) then 
            if (-d tempdir) rm -rf tempdir
            mkdir tempdir
            mv checkmssfile tempdir/.
            cp $file tempdir/.
	    cd tempdir
            tar -xf checkmssfile
            if (${status} != 0) then
               echo "ccsm_l_archive.csh: error trying to extract checkmssfile tar file - exiting"
               exit -1
            endif

            tar -xf $file
            if (${status} != 0) then
               echo "ccsm_l_archive.csh: error trying to extract $file tar file - exiting"
               exit -1
            endif

            rm $file

            #make sure tarring is done consistently throughout so that cmp will validate transfer
            set file_list = `sh -c 'ls *.nc 2> /dev/null'`
            foreach ifile ($file_list)
	       if (-e $file) then
	          tar -uf $file $ifile
                  set stat = $status
               else
                  tar -cf $file $ifile
                  set stat = $status
               endif
               if ( $stat == 0 ) then
		  rm $ifile
               else
		  echo "ccsm_l_archive.csh: error trying to create $file tar file - exiting"
		  exit -1
	       endif
            end

            mv -f $file ../.
            cd ..
            rm -rf tempdir
         endif

	 echo ccsm_mswrite $file ${lsmdir}
	 $UTILROOT/Tools/ccsm_mswrite $file ${lsmdir}/$file
      endif
    endif

    if (-e checkmssfile) rm -f checkmssfile

  endif   # -f file
  end     # file
  date
endif      # -d dirl2
end        # dirl2
endif      # -d dirl1
end        # dirl1

wait
date






