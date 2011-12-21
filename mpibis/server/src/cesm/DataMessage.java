package cesm;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class DataMessage extends Message {

	public final int dest;
	public final int tag;
	public final int count;
	public final int bytes;
	
	public final byte [] data;
	
	DataMessage(DataInputStream in) throws IOException {
	
		super(Protocol.OPCODE_DATA, in);

		dest   = in.readInt();
		tag    = in.readInt();
		count  = in.readInt();
		bytes  = in.readInt();
		
		data = new byte[bytes];
		
		in.readFully(data);
	}
	
	void write(DataOutputStream out) throws IOException {
		super.write(out);
		out.writeInt(dest);
		out.writeInt(tag);
		out.writeInt(count);
		out.writeInt(bytes);
		out.write(data);
	}	
}
	