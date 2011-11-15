package cesm;

import java.io.DataOutputStream;
import java.io.IOException;

public class CommReply extends Message {

    // These a the new virtual communicator values.
    public final int newComm;
    public final int rank;
    public final int size;

    // These are needed to correctly split the real communicator on the host.
    public final int color;
    public final int key;

    // These contain info about the distribution of the virtual communicator.
    public final int flags;
    public final byte [] bitmap;

    CommReply(int comm, int newComm, int rank, int size, int color, int key, int flags, byte [] bitmap) {

        super(Protocol.OPCODE_COMM_REPLY, comm, -1);

        this.newComm = newComm;
        this.rank = rank;
        this.size = size;
        this.color = color;
        this.key = key;

        this.flags = flags;
        this.bitmap = bitmap;
    }

    void write(DataOutputStream out) throws IOException {
        super.write(out);
        out.writeInt(newComm);
        out.writeInt(rank);
        out.writeInt(size);
        out.writeInt(color);
        out.writeInt(key);
        out.writeInt(flags);

        if (size > 0) {
            out.write(bitmap);
        }
    }
}
