package cesm2.messages;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public abstract class ApplicationMessage {



    public final int comm;
    public final int source;

    protected ApplicationMessage(int opcode, DataInputStream in) throws IOException {

       // System.out.println("Reading generic message header");

        this.opcode = opcode;
        comm   = in.readInt();
        source = in.readInt();

    }

    protected ApplicationMessage(int opcode, int comm, int source) {
        this.opcode = opcode;
        this.comm   = comm;
        this.source = source;
    }

    void write(DataOutputStream out) throws IOException {
        out.writeInt(opcode);
        out.writeInt(comm);
        out.writeInt(source);
    }

    public long dataSize() {
        return 0;
    }

    public long headerSize() {
        return 3*4;
    }

    public boolean isData() {
        return false;
    }

    public long size() {
        return headerSize() + dataSize();
    }
}
