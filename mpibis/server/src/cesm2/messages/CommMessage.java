package cesm2.messages;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import cesm2.Protocol;

public class CommMessage extends ApplicationMessage {

    public final int color;
    public final int key;

    CommMessage(DataInputStream in) throws IOException {
        super(Protocol.OPCODE_COMM, in);
        color = in.readInt();
        key = in.readInt();
    }

    void write(DataOutputStream out) throws IOException {
        super.write(out);
        out.writeInt(color);
        out.writeInt(key);
    }	
    
    public long dataSize() { 
        return 2*4;
    }
}
