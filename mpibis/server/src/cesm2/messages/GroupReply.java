package cesm2.messages;

import java.io.DataOutputStream;
import java.io.IOException;

import cesm2.Protocol;

public class GroupReply extends ApplicationMessage {

    public static final int TYPE_ACTIVE     = 0;
    public static final int TYPE_SEPERATIST = 1;
    public static final int TYPE_IDLE       = 2;

    // These a the new virtual communicator values.
    public final int newComm;
    public final int rank;
    public final int size;

    // These contain info about the distribution of the virtual communicator.
    public final int clusterCount;
    public final int flags;
    
    public final int [] coordinators;
    public final int [] clusterSizes;
    public final int [] members;
    
    // This field indicates if the
    public final int type;

    GroupReply(int comm, boolean overlap) {

        super(Protocol.OPCODE_GROUP_REPLY, comm, -1);

        this.newComm = -1;
        this.rank = -1;
        this.size = -1;
        this.flags = -1;
        this.clusterCount = -1;
        this.members = null;
        this.clusterSizes = null;
        this.coordinators = null;
        
        if (overlap) {
            this.type = TYPE_SEPERATIST;
        } else {
            this.type = TYPE_IDLE;
        }
    }

    GroupReply(int comm, int newComm, int rank, int size, int clusterCount, int flags, 
            int [] coordinators, int [] clusterSizes, int [] members) {

        super(Protocol.OPCODE_GROUP_REPLY, comm, -1);

        this.newComm = newComm;
        this.rank = rank;
        this.size = size;
        this.clusterCount = clusterCount;
        this.flags = flags;
        this.coordinators = coordinators;
        this.clusterSizes = clusterSizes;
        this.members = members; 
        this.type = TYPE_ACTIVE;
    }

    void write(DataOutputStream out) throws IOException {

        super.write(out);
        out.writeInt(newComm);
        out.writeInt(rank);
        out.writeInt(size);
        out.writeInt(type);
        out.writeInt(clusterCount);
        out.writeInt(flags);

        for (int i=0;i<clusterCount;i++) { 
            out.writeInt(coordinators[i]);
        }
        
        for (int i=0;i<clusterCount;i++) { 
            out.writeInt(clusterSizes[i]);
        }
        
        if (type == TYPE_ACTIVE) {
            for (int i=0;i<size;i++) { 
                out.writeInt(members[i]);
            }
        }
    }
    
    public long dataSize() { 
        return 4*5 + clusterCount*4*2 + size*4;
    }
}