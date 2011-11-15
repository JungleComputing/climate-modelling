package cesm;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;

public class Communicator {

    private final static boolean SANITY = true;

    private static final int COMM_FLAG_LOCAL  = (1 << 0);
    private static final int COMM_FLAG_REMOTE = (1 << 1);

    private final int communicator;
    private final Connection [] processes;
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
        this.communicator = communicator;
        this.processes = processes;
        this.parent = parent;
        this.size = processes.length;
        this.messages = new Message[size];
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

    private byte [] generateBitmap(String name, Connection [] procs) {

        if (procs == null || procs.length == 0) {
            System.err.println("INTERNAL ERROR: generateBitmap called for empty list!");
            return new byte[0];
        }

        byte [] bitmap = new byte[procs.length];

        for (int i=0;i<procs.length;i++) {

            if (name.equals(procs[i].getClusterName())) {
                // local process
                bitmap[i] = 1;
            } else {
                // remote process
                bitmap[i] = 0;
            }
        }

        return bitmap;
    }

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
                int number = -1;
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
                    number = parent.createCommunicator(procs);

                    // Next, we send a reply to all participants, providing them with the new virtual communicator, its size,
                    // and their new rank. In addition, we need to send a color and rank for the split that needs to be performed
                    // on the 'real communicator', and a flag and bitmap needed by the virtual communicator on the MPI side.

                    // Use a hash map to keep track of the keys (= real ranks) in each of the clusters for each of the
                    // participants.
                    HashMap<String, Integer> realRanks = new HashMap<String, Integer>();

                    // Use a hash map to keep track of the bitmaps we need to generate.
                    HashMap<String, byte []> bitmaps = new HashMap<String, byte []>();

                    // Generate the flags needed by the virtual communicator.
                    int flags = generateFlags(procs);

                    // Send a reply to each participant, generating the appropriate keys and bitmaps for each participant.
                    for (int j=0;j<size;j++) {

                        // Get the connection and cluster name we are sending to
                        Connection c = procs[j];
                        String name = c.getClusterName();

                        // Generate a correct key for this cluster.
                        Integer key = realRanks.get(name);

                        if (key == null) {
                            key = 0;
                        }

                        realRanks.put(c.getClusterName(), key+1);

                        // Generate a correct bitmap for this cluster.
                        byte [] bitmap = bitmaps.get(name);

                        if (bitmap == null) {
                            bitmap = generateBitmap(name, procs);
                            bitmaps.put(name, bitmap);
                        }

                        // Send the reply.
                        c.enqueue(new CommReply(communicator, number, j, size, color, key, flags, bitmap), false);
                    }

                } else {
                    // We must also send a reply to all participants with color -1.
                    // As these will not actually create a new virtual communicator,
                    // we can send a simplified reply.
                    for (CommMessage m : l) {
                        processes[m.source].enqueue(new CommReply(communicator,
                                -1, -1, 0, -1, 0, 0, null), false);
                    }
                }
            }
        }
    }

    private void group() {

        System.out.println("Creating new group from communicator " + communicator);

        int [] ranks = ((GroupMessage) messages[0]).ranks;

        if (SANITY) {
            // Sanity check: all group messages should contain the same ranks array.
            for (int i=1;i<messages.length;i++) {
                if (!Arrays.equals(ranks, ((GroupMessage) messages[i]).ranks)) {
                    System.out.println("ERROR: collective group creation does not have matching parameters! "
                            + Arrays.toString(ranks) + " != " + Arrays.toString(((GroupMessage) messages[i]).ranks));
                    return; // FIXME: This return will hang the program!
                }
            }
        }

        System.out.println("   processes(" + ranks.length + "): " + Arrays.toString(ranks));

        // We gather all connections to the participating machines, and save all connections
        // to the machines that do not participate.
        Connection [] copy = processes.clone();
        Connection [] procs = new Connection[ranks.length];

        for (int i=0;i<ranks.length;i++) {
            procs[i] = copy[ranks[i]];
            copy[ranks[i]] = null;
        }

        // We now send a reply to all processes. Note that some may not participate in the new communicator.

        // We generate a new 'virtual' communicator.
        int number = parent.createCommunicator(procs);

        System.out.println("   new communicator: " + number);

        // Next, we send a reply to all participants, providing them with the new virtual communicator, its size,
        // and their new rank.

        // Use a hash map to keep track of the keys (= real ranks) in each of the clusters for each of the
        // participants.
        HashMap<String, Integer> realRanks = new HashMap<String, Integer>();

        // Use a hash map to keep track of the bitmaps we need to generate.
        HashMap<String, byte []> bitmaps = new HashMap<String, byte []>();

        // Generate the flags needed by the virtual communicator.
        int flags = generateFlags(procs);

        System.out.println("   flags: " + flags);

        // Send a reply to each participant, generating the appropriate keys and bitmaps for each participant.
        for (int j=0;j<procs.length;j++) {

            // Get the connection and cluster name we are sending to
            Connection c = procs[j];
            String name = c.getClusterName();

            // Generate a correct key for this cluster.
            Integer key = realRanks.get(name);

            if (key == null) {
                key = 0;
            }

            realRanks.put(c.getClusterName(), key+1);

            // Generate a correct bitmap for this cluster.
            byte [] bitmap = bitmaps.get(name);

            if (bitmap == null) {
                bitmap = generateBitmap(name, procs);
                bitmaps.put(name, bitmap);
            }

            System.out.println("   reply(" + j + "): " + key + " " + name + " " + procs.length + " " + flags + " " + Arrays.toString(bitmap));

            // Send the reply.
            c.enqueue(new GroupReply(communicator, number, j, procs.length, flags, bitmap), false);
        }

        // Send a reply to each process that does not participate, as they may still need to perform a some local collectives.
        // We check in bitmaps to see if (part of) a cluster is participating in the communicators. We store this result in
        // the size field of the GroupReply
        for (int j=0;j<copy.length;j++) {
            if (copy[j] != null) {
                System.out.println("   reply(" + j + "): " + bitmaps.containsKey(copy[j].getClusterName()));
                copy[j].enqueue(new GroupReply(communicator, bitmaps.containsKey(copy[j].getClusterName())), false);
            }
        }
    }

    private void dup() {

        System.out.println("Creating dup of communicator " + communicator);

        // We generate a new 'virtual' communicator.
        int number = parent.createCommunicator(processes);

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

        if (m.dest > processes.length) {
            System.err.println("ERROR: Unable to deliver message to " + m.dest
                    + " on comm " + communicator);
            return;
        }

        processes[m.dest].enqueue(m, true);
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
        default:
            System.err.println("INTERNAL ERROR: unknown message type " +
                    m.opcode);
        }
    }

}
