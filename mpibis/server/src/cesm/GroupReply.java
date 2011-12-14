package cesm;

import java.io.DataOutputStream;
import java.io.IOException;

public class GroupReply extends Message {

    public static final int TYPE_ACTIVE     = 0;
    public static final int TYPE_SEPERATIST = 1;
    public static final int TYPE_IDLE       = 2;

    // These a the new virtual communicator values.
    public final int newComm;
    public final int rank;
    public final int size;

    // These contain info about the distribution of the virtual communicator.
    public final int flags;
    public final int [] members;

    // This field indicates if the
    public final int type;

    GroupReply(int comm, boolean overlap) {

        super(Protocol.OPCODE_GROUP_REPLY, comm, -1);

        this.newComm = -1;
        this.rank = -1;
        this.size = -1;
        this.flags = -1;
        this.members = null;

        if (overlap) {
            this.type = TYPE_SEPERATIST;
        } else {
            this.type = TYPE_IDLE;
        }
    }

    GroupReply(int comm, int newComm, int rank, int size, int flags, int [] members) {

        super(Protocol.OPCODE_GROUP_REPLY, comm, -1);

        this.newComm = newComm;
        this.rank = rank;
        this.size = size;
        this.flags = flags;
        this.members = members;
        this.type = TYPE_ACTIVE;
    }

    void write(DataOutputStream out) throws IOException {

        super.write(out);
        out.writeInt(newComm);
        out.writeInt(rank);
        out.writeInt(size);
        out.writeInt(type);
        out.writeInt(flags);

        if (type == TYPE_ACTIVE) {
            for (int i=0;i<size;i++) { 
                out.writeInt(members[i]);
            }
        }
    }
}
