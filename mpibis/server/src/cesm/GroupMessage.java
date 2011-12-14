package cesm;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class GroupMessage extends Message {

    public final int [] ranks;

    GroupMessage(DataInputStream in) throws IOException {

        super(Protocol.OPCODE_GROUP, in);

        System.out.println("Reading group message");

        int size = in.readInt();

        ranks = new int[size];

        for (int i=0;i<size;i++) { 
            ranks[i] = in.readInt();
        }
    }

    void write(DataOutputStream out) throws IOException {
        super.write(out);
        out.writeInt(ranks.length);

        for (int i=0;i<ranks.length;i++) { 
            out.writeInt(ranks[i]);
        }
    }	
}
