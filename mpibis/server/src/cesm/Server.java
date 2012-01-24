package cesm;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;

public class Server {

    static class AcceptThread extends Thread { 

        private final Server parent;
        private final Socket s;
        
        AcceptThread(Server parent, Socket s) throws IOException { 
            this.parent = parent;
            this.s = s;
        }
            
        public void run() {
            try { 
                new Connection(parent, s);
            } catch (Exception e) {
                System.err.println("Failed to initiate new connection!" + e);
                e.printStackTrace(System.err);
            }
        }
    }

    private final int clusterCount;
    private final int port;

    private final Cluster [] clusters;
    private final int [] clusterSizes;
    private int totalProcesses;

    private ServerSocket ss;

    private boolean signupComplete = false;

    private ArrayList<Communicator> communicators = new ArrayList<Communicator>();

    public Server(int clusterCount, int port) {
        this.clusterCount = clusterCount;
        this.port = port;
        this.clusterSizes = new int[clusterCount];
        this.clusters = new Cluster[clusterCount];
    }

    private Cluster getCluster(int index) {
        synchronized (clusters) {
            return clusters[index];
        }
    }

    protected void deliver(Message m) {

        Communicator c = null;

        // FIXME: potentially huge bottleneck!    
        synchronized (this) {
            if (m.comm >= communicators.size()) {
                System.err.println("Cannot deliver message, unknown communicator " + m.comm);
                return;
            }

            c = communicators.get(m.comm);
        }

        if (c == null) {
            System.err.println("Cannot deliver message, unknown communicator " + m.comm);
            return;
        }

        c.deliver(m);
    }

    protected Communicator createCommunicator(Connection [] processes) {
        Communicator comm;
        
        synchronized (this) {
            int c = communicators.size();
            comm = new Communicator(this, c, processes);
            communicators.add(c, comm);
        }

        System.out.println("Created new communicator: " + comm.getNumber());
        return comm;
    }

    private boolean signupComplete() {

        for (int i=0;i<clusters.length;i++) {

            Cluster tmp = getCluster(i);

            if (tmp == null) {
                return false;
            }

            if (!tmp.complete()) {
                return false;
            }
        }

        // If we reach this point all cluster have signed up!
        ArrayList<Connection> c = new ArrayList<Connection>();

        for (int i=0;i<clusters.length;i++) {
            Cluster tmp = getCluster(i);
            clusterSizes[i] = tmp.localCount;
            tmp.getConnections(c);
        }

        totalProcesses = c.size();

        Communicator com = new Communicator(this, 0,
                (Connection[]) c.toArray(new Connection[c.size()]));

        System.out.println("Created communicator 0 with " + totalProcesses
                + " processes");

        synchronized (this) {
            // communicators 0, 1 and 2 are reserved for COMM_WORLD, COMM_SELF and COMM_NULL
            communicators.add(0, com);
            communicators.add(1, null);
            communicators.add(2, null);

            signupComplete = true;
            notifyAll();
        }

        return true;
    }

    synchronized int [] waitUntilSignupComplete() {

        while (!signupComplete) {
            try {
                wait();
            } catch (InterruptedException e) {
                // ignored
            }
        }

        return clusterSizes;
    }

    Cluster getCluster(int clusterRank, int clusterSize, int clusterCount, String clusterName) throws Exception {

        if (this.clusterCount != clusterCount) {
            throw new Exception("Number of clusters does not match (" + clusterCount + " != " + this.clusterCount + ")");
        }

        Cluster tmp;

        synchronized (clusters) {
            if (clusters[clusterRank] == null) {
                System.out.println("Added cluster " + clusterName + " (" + clusterRank
                        + " of " + clusterCount + ") of size " + clusterSize);
                clusters[clusterRank] = new Cluster(clusterName, clusterSize, clusterRank);
            }

            tmp = clusters[clusterRank];
        }

        return tmp;
    }

    public void run() throws Exception {

        boolean done = false;

        ss = new ServerSocket(port);
        ss.setSoTimeout(1000);

        while (!done) {

            try {
                Socket s = ss.accept();
                new AcceptThread(this, s).start();

            } catch (SocketTimeoutException e) {
                // ignored!
            }

            done = signupComplete();
        }

        try {
            ss.close();
        } catch (Exception e) {
            // ignored
        }
    }


    public static void main(String [] args) {

        try {
            int port = 45678;
            int clusters = 0;

            for (int i=0;i<args.length;i++) {

                if (args[i].equals("--port")) {
                    if (++i < args.length) {
                        port = Integer.parseInt(args[i]);
                    } else {
                        System.err.println("Missing parameter for --port");
                    }
                } else if (args[i].equals("--clusters")) {
                    if (++i < args.length) {
                        clusters = Integer.parseInt(args[i]);
                    } else {
                        System.err.println("Missing parameter for --cluster");
                    }
                } else {
                    System.err.println("Unknown option " + args[i]);
                }
            }

            if (port <= 0 || port > 65535) {
                System.err.println("Illegal port number " + port);
            }

            if (clusters <= 0 || clusters > 100) {
                System.err.println("Illegal clusters count " + clusters);
            }

            new Server(clusters, port).run();

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
