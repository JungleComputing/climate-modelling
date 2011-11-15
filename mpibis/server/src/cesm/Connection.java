package cesm;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.LinkedList;

public class Connection implements Protocol {

    class SenderThread extends Thread {
        public void run() {
            try {
                connectionSetup();

                receiver.start();

                boolean done = false;

                while (!done) {
                    sendMessage();
                }

            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
    }

    class ReceiverThread extends Thread {
        public void run() {
            try {
                boolean done = false;

                while (!done) {
                    receiveMessage();
                }
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
    }

    private final SenderThread sender;
    private final ReceiverThread receiver;

    private final Server parent;

    private final Socket s;
    private final DataInputStream in;
    private final DataOutputStream out;

    private String clusterName;
    private int clusterRank;
    private int clusterSize;
    private int localRank;
    private int localSize;

    private LinkedList<Message> incoming = new LinkedList<Message>();
    private boolean done = false;

    Connection(Server parent, Socket s) throws IOException {
        this.parent = parent;
        this.s = s;

        //in = new DataInputStream(new NoisyInputStream(s.getInputStream()));
        // FIXME: Add buffering here when everything runs OK.
        in = new DataInputStream(new BufferedInputStream(s.getInputStream()));
        out = new DataOutputStream(new BufferedOutputStream(s.getOutputStream()));

        sender = new SenderThread();
        receiver = new ReceiverThread();

        sender.start();
    }

    void done() {
        synchronized (incoming) {
            done = true;
            incoming.notifyAll();
        }
    }

    void enqueue(Message m, boolean tail) {
        synchronized (incoming) {

            if (tail) {
                incoming.addLast(m);
            } else {
                incoming.addFirst(m);
            }

            incoming.notifyAll();
        }
    }

    Message dequeue() {
        synchronized (incoming) {

            while (!done && incoming.size() == 0) {
                try {
                    incoming.wait();
                } catch (InterruptedException e) {
                    // ignored
                }
            }

            if (done) {
                return null;
            } else {
                return incoming.removeFirst();
            }
        }
    }

    private void readHandshake() throws Exception {

        int opcode = in.readInt();

        if (opcode != OPCODE_HANDSHAKE) {
            throw new Exception("Illegal opcode " + opcode);
        }

        localRank = in.readInt();
        localSize = in.readInt();
        clusterRank = in.readInt();
        clusterSize = in.readInt();

        int len = in.readInt();

        byte [] tmp = new byte[len];

        in.readFully(tmp);

        clusterName = new String(tmp);
    }

    private void writeHandshake(int [] clusterSizes) throws Exception {

        out.write(OPCODE_HANDSHAKE_ACCEPTED);

        for (int i=0;i<clusterSizes.length;i++) {
            out.writeInt(clusterSizes[i]);
        }

        out.flush();
    }

    private void close() {
        try {
            in.close();
        } catch (Exception e) {
            // ignore
        }

        try {
            out.close();
        } catch (Exception e) {
            // ignore
        }

        try {
            s.close();
        } catch (Exception e) {
            // ignore
        }
    }

    private void connectionSetup() throws Exception {
        readHandshake();

        Cluster c = parent.getCluster(clusterRank, localSize, clusterSize, clusterName);

        c.addConnection(localRank, localSize, clusterName, this);

        int [] clusterSizes = parent.waitUntilSignupComplete();
        writeHandshake(clusterSizes);
    }

    private boolean receiveMessage() throws Exception {

        System.out.println("Waiting for message");

        int opcode = in.readInt();

        Message m = null;

        switch (opcode) {
        case OPCODE_DATA:
            m = new DataMessage(in);
            break;

        case OPCODE_COMM:
            m = new CommMessage(in);
            break;

        case OPCODE_GROUP:
            m = new GroupMessage(in);
            break;

        case OPCODE_DUP:
            m = new DupMessage(in);
            break;

        default:
            System.out.println("GOT illegal opcode " + opcode);
            throw new Exception("Illegal opcode " + opcode);
        }

        parent.deliver(m);
        return true;
    }

    private boolean sendMessage() throws Exception {

        Message m = dequeue();

        if (m == null) {
            return false;
        }

        System.out.println("Forwarding message from " + m.source + " to " +
                clusterName + ":" + localRank);

        m.write(out);
        out.flush();
        return true;
    }

    public String getClusterName() {
        return clusterName;
    }

    public int getClusterRank() {
        return clusterRank;
    }

    public int getClusterSize() {
        return clusterSize;
    }

    public int getLocalRank() {
        return localRank;
    }

    public int getLocalSize() {
        return localSize;
    }



}
