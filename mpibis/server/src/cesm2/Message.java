package cesm2;

import java.io.Serializable;

public class Message implements Serializable {

    public final int opcode;

    public final int sourcePID = 0;
    public final int destinationPID = 0;

    public void Message(int opcode, int sourcePID, int destinationPID) {
        this.sourcePID = sourcePID;
        this.destinationPID = destinationPID
    }


}
