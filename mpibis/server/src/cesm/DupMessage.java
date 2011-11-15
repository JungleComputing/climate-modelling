package cesm;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class DupMessage extends Message {

    DupMessage(DataInputStream in) throws IOException {
        super(Protocol.OPCODE_DUP, in);
        System.out.println("Reading dup message");
    }

    void write(DataOutputStream out) throws IOException {
        super.write(out);
    }
}
