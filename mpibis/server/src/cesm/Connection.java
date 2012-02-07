package cesm;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.util.LinkedList;

public class Connection extends Thread implements Protocol {

    class SenderThread extends Thread {
        public void run() {
            try {
                boolean more = true;

                while (more) {
                    more = sendMessage();
                }
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
    }

    private final SenderThread sender;

    private final Server parent;

    private final Socket s;
    private final DataInputStream in;
    private final DataOutputStream out;

    public final String clusterName;
    public final int clusterRank;
    public final int clusterSize;
    public final int localRank;
    public final int localSize;

    public final int pid;
    public final String pidAsString;
    
    private final LinkedList<Message> incoming = new LinkedList<Message>();
    private boolean done = false;

    public long bytesSend; 
    public long bytesReceived; 
    
    public long messagesSend; 
    public long messagesReceived; 
        
    Connection(Server parent, Socket s) throws Exception {
        this.parent = parent;
        this.s = s;

	System.out.println("Got connection from " + s);

        //in = new DataInputStream(new NoisyInputStream(s.getInputStream()));
        in = new DataInputStream(new BufferedInputStream(s.getInputStream()));
        out = new DataOutputStream(new BufferedOutputStream(s.getOutputStream()));

        // Read the incoming handshake.
        int opcode = in.readInt();

        if (opcode != OPCODE_HANDSHAKE) {
            throw new Exception("Illegal opcode " + opcode);
        }

        localRank = in.readInt();
        localSize = in.readInt();
        clusterRank = in.readInt();
        clusterSize = in.readInt();

        pid = ((clusterRank & 0xFF) << 24) | (localRank & 0x00FFFFFF);   
    
        pidAsString = (clusterRank & 0xFF) + ":" + (localRank & 0x00FFFFFF);   
        
	System.out.println(pidAsString + " is " + localRank + "/" + localSize + " " + clusterRank + "/" + clusterSize);
       
        int len = in.readInt();

	System.out.println(pidAsString + " read len " + len);

        byte [] tmp = new byte[len];

        in.readFully(tmp);

        clusterName = new String(tmp);

	System.out.println(pidAsString + " cluster is " + clusterName);

        // Register ourselves at our cluster.
        Cluster c = parent.getCluster(clusterRank, localSize, clusterSize, clusterName);
        c.addConnection(localRank, localSize, clusterName, this);

	System.out.println(pidAsString + " waiting until signup complete.");

        // Wait until everyone has registered.
        int [] clusterSizes = parent.waitUntilSignupComplete();

	System.out.println(pidAsString + " writing output handshake");
        
        // Write the output handshake.
        out.write(OPCODE_HANDSHAKE_ACCEPTED);

        for (int i=0;i<clusterSizes.length;i++) {
            out.writeInt(clusterSizes[i]);
        }

        out.flush();
                
        // Start a separate sender thread.
        sender = new SenderThread();
        sender.start();
     
        System.out.println(pidAsString + " init done!");
        
        // Start my thread to receive incoming messages.
        start();
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
    
    private boolean receiveMessage() throws Exception {

        System.out.println(pidAsString + " - Waiting for message");

        int opcode = in.readInt();

        Message m = null;

        switch (opcode) {
        case OPCODE_DATA:
            System.out.println(pidAsString + " - Reading DATA message");
            m = new DataMessage(OPCODE_DATA, in);
            System.out.println(pidAsString + " - DATA message read");
            break;

        case OPCODE_COLLECTIVE_BCAST:
            System.out.println(pidAsString + " - Reading BCAST message");
            m = new DataMessage(OPCODE_COLLECTIVE_BCAST, in);
            System.out.println(pidAsString + " - BCAST message read");
            break;
            
        case OPCODE_COMM:
            System.out.println(pidAsString + " - Reading COMM message");
            m = new CommMessage(in);
            break;

        case OPCODE_GROUP:
            System.out.println(pidAsString + " - Reading GROUP message");
            m = new GroupMessage(in);
            break;

        case OPCODE_DUP:
            System.out.println(pidAsString + " - Reading DUP message");
            m = new DupMessage(in);
            break;

        case OPCODE_TERMINATE:
            System.out.println(pidAsString + " - Reading TERMINATE message");
            m = new TerminateMessage(in);
            break;
            
        case OPCODE_CLOSE_LINK:
            done();           
            return false;
            
        default:
            System.out.println(pidAsString + " GOT illegal opcode " + opcode);
            throw new Exception("Illegal opcode " + opcode + " read by " + pidAsString);
        }

        bytesReceived += m.size();
        messagesReceived++;
        
        parent.deliver(m);
        return true;
    }

    private boolean sendMessage() throws Exception {

        Message m = dequeue();

        if (m == null) {
            return false;
        }

        System.out.println(pidAsString + " Forwarding message from " + m.source);

        m.write(out);
        out.flush();
        
        bytesSend += m.size();
        messagesSend++;
        
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

    public String printStatistics() { 
        
        StringBuilder sb = new StringBuilder();

        sb.append(pidAsString + " " + localRank + " " + localSize + " " + 
                  clusterName + " " + clusterRank + " " + clusterSize + " " + 
                  messagesReceived + " " + messagesSend + " " + 
                  bytesReceived + " " + bytesSend);
        
        return sb.toString();
    }
    
    public void run() {
        try {

            boolean more = true;

            while (more) {
                more = receiveMessage();
            }

            // If we are done, we wait for the sender to join.
            try { 
                sender.join();
            } catch (Exception e) {
                System.err.println("Sender thread failed to join!");
                e.printStackTrace(System.err);
            }
        
            // Next we close the socket.
            close();
            
            // Finally we print some statistics
            System.out.println("Connection closed: " + printStatistics());
        
        } catch (Exception e) {
            System.err.println("Connection thread failed!");
            e.printStackTrace(System.err);
        }
    }

}
