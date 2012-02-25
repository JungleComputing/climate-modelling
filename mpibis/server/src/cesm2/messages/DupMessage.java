package cesm2.messages;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import cesm2.Protocol;

public class DupMessage extends ApplicationMessage {

    DupMessage(DataInputStream in) throws IOException {
        super(Protocol.OPCODE_DUP, in);
    }

    void write(DataOutputStream out) throws IOException {
        super.write(out);
    }
}
