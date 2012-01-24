package cesm;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;

public class Communicator {

    private final static boolean SANITY = true;

    private static final int COMM_FLAG_LOCAL  = (1 << 0);
    private static final int COMM_FLAG_REMOTE = (1 << 1);

    private final int communicator;
    private final Connection [] processes;
    private final int [] coordinatorRanks;
    
    private final HashMap<Integer, Connection> pids;
    
    private final int size;
    private final Server parent;

    private final Message [] messages;
    private int participants = 0;

    private class ColorComparator implements Comparator<CommMessage> {

        @Override
        public int compare(CommMessage o1, CommMessage o2) {

            if (o1.key < o2.key) {
                return -1;
            } else if (o1.key > o2.key) {
                return 1;
            }

            // The keys are the same, so compare based on current rank.
            if (o1.source < o2.source) {
                return -1;
            } else if (o1.source > o2.source) {
                return 1;
            }

            // Should never happen ?
            return 0;
        }
    }

    private final ColorComparator colorComparator = new ColorComparator();

    Communicator(Server parent, int communicator, Connection [] processes) {
        
        if (SANITY) { 
            // Make sure the processes array doesn't contain any holes!
            if (processes == null) { 
                System.out.println("ERROR: processes array null at communicator creation!");
                throw new IllegalArgumentException("Processes array null at communicator creation");
            }
           
            for (int i=0;i<processes.length;i++) { 
                if (processes[i] == null) { 
                    System.out.println("ERROR: processes array entry " + i + " null at communicator creation!");
                    throw new IllegalArgumentException("Processes array entry " + i + " null at communicator creation");
                }
            } 
        }
        
        this.communicator = communicator;
        this.processes = processes;
        this.parent = parent;
        this.size = processes.length;
        this.messages = new Message[size];

        pids = new HashMap<Integer, Connection>();
       
        ArrayList<Integer> coordinatorRanks = new ArrayList<Integer>();
        HashSet<Integer> clusters = new HashSet<Integer>();
        
        for (int i=0;i<size;i++) {
            int pid = processes[i].pid;
            
            pids.put(pid, processes[i]);
            
            int cluster = processes[i].clusterRank; 
            
            if (!clusters.contains(cluster)) { 
                clusters.add(cluster);
                coordinatorRanks.add(i);
            }
        }
      
        this.coordinatorRanks = new int[coordinatorRanks.size()];
        
        for (int i=0;i<coordinatorRanks.size();i++) { 
            this.coordinatorRanks[i] = coordinatorRanks.get(i);
        }
    }

    public int getNumber() { 
        return communicator;
    }

    public int [] getCoordinatorRanks() {
        return coordinatorRanks;
    }
    
    private int generateFlags(Connection [] procs) {

        if (procs == null || procs.length == 0) {
            System.err.println("INTERNAL ERROR: generateFlags called for empty list!");
            return 0;
        }

        String name = procs[0].getClusterName();

        for (int i=1;i<procs.length;i++) {
            if (!name.equals(procs[i].getClusterName())) {
                return (COMM_FLAG_LOCAL | COMM_FLAG_REMOTE);
            }
        }

        return COMM_FLAG_LOCAL;
    }
    
    private int [] generateMembers(Connection [] procs) {

        if (procs == null || procs.length == 0) {
            System.err.println("INTERNAL ERROR: generateMembers called for empty list!");
            return new int[0];
        }

        int [] members = new int[procs.length];

        for (int i=0;i<procs.length;i++) {
            members[i] = procs[i].pid;
        }

        return members;
    }

    /*
    private int countClusters(Connection [] procs) {
        
        HashSet<Integer> clusterCount = new HashSet<Integer>();
        
        for (Connection c : procs) { 
            clusterCount.add(c.clusterRank);
        }
        
        return clusterCount.size();
    }
    */
    
    // This implements the split operation. Note that dup and create are just
    // special cases of split, and therefore handled by the same code.
    private void split() {

        // First gather all messages sharing a colors together in a list.
        HashMap<Integer, LinkedList<CommMessage>> tmp =
                new HashMap<Integer, LinkedList<CommMessage>>();

        for (int i=0;i<size;i++) {

            CommMessage m = (CommMessage) messages[i];

            Integer color = m.color;

            LinkedList<CommMessage> l = tmp.get(color);

            if (l == null) {
                l = new LinkedList<CommMessage>();
                tmp.put(color, l);
            }

            l.add(m);
        }

        // Next, sort each list on <preferred rank / existing rank> and send a
        // reply. Note that the color -1 is reserved for machines that don't
        // participate.
        for (Integer color : tmp.keySet()) {

            LinkedList<CommMessage> l = tmp.get(color);

            if (l == null || l.isEmpty()) {
                System.err.println("INTERNAL ERROR: Split created empty list!");
            } else {
                // Create a new communicator, provided the color >= 0 (color -1 is used for non-participating processes).
                int size = l.size();

                if (color >= 0) {

                    // We first sort the participants on their requested rank.
                    Collections.sort(l, colorComparator);

                    // Next, we gather all connections to the participating machines.
                    Connection [] procs = new Connection[size];

                    int i=0;

                    for (CommMessage m : l) {
                        procs[i++] = processes[m.source];
                    }

                    // We generate a new 'virtual' communicator.
                    Communicator com = parent.createCommunicator(procs);

                    // Next, we send a reply to all participants, providing them with the new virtual communicator, its size,
                    // and their new rank. In addition, we need to send a color and rank for the split that needs to be performed
                    // on the 'real communicator', and a flag and member set needed by the virtual communicator on the MPI side.

                    // Use a hash map to keep track of the desired local ranks in each of the clusters for each of the 
                    // participants. These are needed to perform the local split.
                    HashMap<String, Integer> localRanks = new HashMap<String, Integer>();

                    // Generate the flags needed by the virtual communicator.
                    int flags = generateFlags(procs);

                    int number = com.getNumber();
                    
                    int [] coordinators = com.getCoordinatorRanks();
                   
                    int [] members = generateMembers(procs);
                    
                    // Send a reply to each participant, generating the appropriate local rank for each participant.
                    for (int j=0;j<size;j++) {

                        // Get the connection and cluster name we are sending to
                        Connection c = procs[j];
                        String name = c.getClusterName();

                        // Generate a correct local for this specific cluster.
                        Integer key = localRanks.get(name);

                        if (key == null) {
                            key = 0;
                        }

                        localRanks.put(c.getClusterName(), key+1);
                       
                        // Send the reply.
                        c.enqueue(new CommReply(communicator, number, j, size, color, key, coordinators.length, flags, coordinators, members), false);
                    }

                } else {
                    // We must also send a reply to all participants with color -1.
                    // As these will not actually create a new virtual communicator,
                    // we can send a simplified reply.
                    for (CommMessage m : l) {
                        processes[m.source].enqueue(new CommReply(communicator,
                                -1, -1, 0, -1, 0, 0, 0, null, null), false);
                    }
                }
            }
        }
    }

    private String printPID(int pid) { 
        return ((pid & 0xFF000000) >> 24) + ":" + (pid & 0xFFFFFF); 
    }
        
    private String printPIDs(int [] pids) { 
        
        StringBuilder sb = new StringBuilder("[ ");
        
        for (int i=0;i<pids.length;i++) { 
            
            sb.append(printPID(pids[i]));
            
            if (i != pids.length-1) { 
                sb.append(", ");
            }
        }
        
        sb.append(" ]");
  
        return sb.toString();
    }
    
    private void group() {

        System.out.println("Creating new group from communicator " + communicator);

        int [] group = ((GroupMessage) messages[0]).pids;

        if (SANITY) {
            // Sanity check: all group messages should contain the same ranks array.
            for (int i=1;i<messages.length;i++) {
                if (!Arrays.equals(group, ((GroupMessage) messages[i]).pids)) {
                    System.out.println("ERROR: collective group creation does not have matching parameters! "
                            + Arrays.toString(group) + " != " + Arrays.toString(((GroupMessage) messages[i]).pids));
                    return; // FIXME: This return will hang the program!
                }
            }
        }

        System.out.println("   processes(" + group.length + "): " + printPIDs(group));

        // We gather all connections to the participating machines, and save all connections
        // to the machines that do not participate.
        HashMap<Integer, Connection> tmp = new HashMap<Integer, Connection>();
        tmp.putAll(pids);
        
        Connection [] used = new Connection[group.length];

        for (int i=0;i<group.length;i++) {
            used[i] = tmp.remove(group[i]);
        }
        
        // We now send a reply to all processes. Note that some may not participate in the new communicator.

        // We generate a new 'virtual' communicator.
        Communicator com = parent.createCommunicator(used);

        int number = com.getNumber();
        int [] coordinators = com.getCoordinatorRanks();
                     
        System.out.println("   new communicator: " + number);

        // Next, we send a reply to all participants, providing them with the new virtual communicator, its size,
        // and their new rank.

        // Generate the flags needed by the virtual communicator.
        int flags = generateFlags(used);

        System.out.println("   flags: " + flags);

        // Generate a correct members array for this cluster.
        int [] members = generateMembers(used);
        
        System.out.println("   group reply: " + number + " " + flags + " " 
                + coordinators.length + " " + Arrays.toString(coordinators) 
                + members.length + " " + flags + " " + printPIDs(members));
        
        // We need to figure out which cluster do and which don't participate. Those that don't do not need to create a local 
        // communicator.
        HashSet<String> participatingCluster = new HashSet<String>();
        
        // Send a reply to each participant, generating the appropriate keys and bitmaps for each participant.
        for (int j=0;j<used.length;j++) {        
            
            // Get the connection and cluster name we are sending to
            Connection c = used[j];
            String name = c.getClusterName();
        
            // Add the cluster to the set of participants.
            participatingCluster.add(name);
            
            System.out.println("        sending group info to " + j + " " + printPID(c.pid) + " at " + name);
            
            // Create and send the reply.
            c.enqueue(new GroupReply(communicator, number, j, members.length, coordinators.length, flags, coordinators, members), 
                    false);
        }

        // Send a reply to each process that does not participate, as they may still need to perform a some local collectives.
        // We check in bitmaps to see if (part of) a cluster is participating in the communicators. We store this result in
        // the size field of the GroupReply
        int j=0;
        
        for (Connection c : tmp.values()) {
            if (c != null) {
                String name = c.getClusterName();
                boolean participant = participatingCluster.contains(name);
            
                System.out.println("        sending participant info to " + j++ + " " + printPID(c.pid) + " at " + name 
                        + "(" + participant + ")");
                
                c.enqueue(new GroupReply(communicator, participant), false);
            }
        }
    }

    private void dup() {

        System.out.println("Creating dup of communicator " + communicator);

        // We generate a new 'virtual' communicator.
        int number = parent.createCommunicator(processes).getNumber();

        System.out.println("   dup communicator: " + communicator + " -> " + number);

        // Next, we send a reply to all participants, providing them with the new virtual communicator.
        DupReply reply = new DupReply(communicator, number);

        for (int j=0;j<processes.length;j++) {
            processes[j].enqueue(reply, false);
        }
    }

    private void processMessages() {

        int opcode = messages[0].opcode;

        if (SANITY) {
            // Sanity check: see if all opcodes match
            for (int i=1;i<messages.length;i++) {
                if (messages[i].opcode != opcode) {
                    System.out.println("ERROR: opcode mismatch in collective communicator operation! " + opcode + " != " + messages[1].opcode);
                    return; //  FIXME: This return will hang the program!
                }
            }
        }

        switch (opcode) {
        case Protocol.OPCODE_COMM:
            split();
            break;

        case Protocol.OPCODE_GROUP:
            group();
            break;

        case Protocol.OPCODE_DUP:
            dup();
            break;

        default:
            System.out.println("ERROR: unknown opcode collective communicator operation! " + opcode);
            return; // FIXME: This return will hang the program!
        }
    }

    private synchronized void process(Message m) {

        // First check the message is legal
        if (m.source < 0 || m.source >= size) {
            System.err.println("ERROR: Unknown rank " + m.source +
                    " for operation on comm " + communicator);
        }

        messages[m.source] = m;
        participants++;

        // If all participant are in, we perform the operation.
        if (participants == size) {
            processMessages();

            // Reset administration.
            participants = 0;

            for (int i=0;i<size;i++) {
                messages[i] = null;
            }
        }
    }

    void deliver(DataMessage m) {
        
        // Simply enqueue the message at the destination
        if (m.dest > processes.length) {
            System.err.println("ERROR: Unable to deliver message to " + m.dest
                    + " on comm " + communicator);
            return;
        }

        processes[m.dest].enqueue(m, true);
    }

    void bcast(DataMessage m) {
        
        // Enqueue the message at each of the cluster coordinators, 
        // but exclude the cluster of the root.
        
        // FIXME: shouldn't we always use pids here?
        Connection source = processes[m.dest]; // pids.get(m.dest);
        
        if (source == null) {
            System.out.println("ERROR: bcast target " + m.dest + " not found! -- DROPPING MESSAGE!!");
            return;
        }
        
        for (int i=0;i<coordinatorRanks.length;i++) {
            
            Connection c = processes[coordinatorRanks[i]];
            
            if (c.clusterRank != source.clusterRank) { 
                System.out.println("Enqueuing BCAST at cluster coordinator " + printPID(c.pid) + " of comm " + communicator);
                c.enqueue(m, true); 
            } else { 
                System.out.println("SKIP Enqueuing BCAST at cluster coordinator " + printPID(c.pid) + " of comm " + communicator);
            }
        }
    }
    
    void deliver(Message m) {

        switch (m.opcode) {
        case Protocol.OPCODE_COMM:
        case Protocol.OPCODE_GROUP:
        case Protocol.OPCODE_DUP:            
            process(m);
            break;
        case Protocol.OPCODE_DATA:
            deliver((DataMessage)m);
            break; 
        case Protocol.OPCODE_COLLECTIVE_BCAST:
            bcast((DataMessage)m);
            break;
        default:
            System.err.println("INTERNAL ERROR: unknown message type " + m.opcode);
        }
    }

}
