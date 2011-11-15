package generator;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.StringTokenizer;

public class Main {
	
	private static class TypePrinter {
		
		final String format;
		final String function;
		
		TypePrinter(String format) { 
			this(format, null);
		}
		
		TypePrinter(String format, String function) { 
			this.format = format;
			this.function = function;					
		}
		
		String getFormat() { 
			return format;
		}
		
		String getValue(String name) {
			
			if (function == null) { 
				return name;
			}
			
			return function + "(" + name + ")";
		}		
	}
	
	private static class ErrorPrinter extends TypePrinter {
		
		ErrorPrinter() { 
			super(null, null);
		}
		
		String getFormat() { 
			fatal("Cannot print this type!", null);
			return null;
		}
		
		String getValue(String name) {
			fatal("Cannot print this type!", null);
			return null;
		}
	} 
		
	private static class UnknownTypePrinter extends TypePrinter {
		
		UnknownTypePrinter() { 
			super("%p", null);
		}
		
		String getValue(String name) {			
			return "(void *) " + name;
		}
	}
		
	private static class Type { 
		final String name;
		final TypePrinter printer;
		
		Type(String name, TypePrinter printer) { 
			this.name = name;
			this.printer = printer;
		}
		
		String getFormat() {
			return printer.getFormat();
		}
		
		String getValue(String name) {
			return printer.getValue(name);
		}
	}
	
	private static class Parameter { 
		
		final boolean pointer;
		final boolean array;
		
		final Type type;
		final String name;

		Parameter(Type type, String name, boolean pointer, boolean array) {
			this.type = type;
			this.name = name;
			this.pointer = pointer;
			this.array = array;
		}				
	}
		
	private static class Function { 
		
		final Type ret;
		final String name;
		
		final Parameter [] parameters;
				
		Function(Type ret, String name, Parameter [] parameters) { 
			this.ret = ret;
			this.name = name;
			this.parameters = parameters;
		}

		String cleanName(String name) { 
			StringTokenizer tok = new StringTokenizer(name, " *[]");
			return tok.nextToken().trim();
		}

		void generateHeader(boolean ibis, boolean colon) { 
			
			if (ibis) { 
				System.out.print(ret.name + " I" + name + " ( ");
			} else {
				System.out.print(ret.name + " " + name + " ( ");
			}
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				System.out.print(p.type.name + " " + p.name);
				
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
			
			if (colon) { 
				System.out.println(" );");
			} else {
				System.out.println(" )");
			}
		}
		
		void generateTrace() { 

			// Print the trace statement
			System.out.println("#ifdef TRACE_CALLS");
			
			System.out.print("   INFO(0, \"" + name + "\",\"(");
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(p.type.name + " " + p.name + "=%p");
				} else { 
					System.out.print(p.type.name + " " + p.name + "=" + p.type.getFormat());
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
		
			System.out.print(")\"");
			
			if (parameters.length > 0) { 
				System.out.print(", ");					
			}
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(cleanName(p.name));
				} else { 
					System.out.print(p.type.getValue(p.name));
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}

			System.out.println(");");			
			System.out.println("#endif // TRACE_CALLS\n");
		}
		
		void generateInterceptCall() { 

			System.out.println("#ifdef IBIS_INTERCEPT");
			System.out.print("   int error = I" + name + "(");
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(cleanName(p.name));
				} else { 
					System.out.print(p.name);
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
			
			System.out.println(");");			
			System.out.println("#else");
			System.out.print("   int error = P" + name + "(");
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(cleanName(p.name));
				} else { 
					System.out.print(p.name);
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
			
			System.out.println(");");			
			System.out.println("#endif // IBIS_INTERCEPT\n");
			
			System.out.println("#ifdef TRACE_ERRORS");
			System.out.println("   if (error != MPI_SUCCESS) {");			
			System.out.println("      ERROR(0, \"" + name + " failed (%d)!\", error);");			
			System.out.println("   }");			
			System.out.println("#endif // TRACE_ERRORS");

			System.out.println("   return error;");			

		}

		void generateErrorCall() { 

			// Print the trace statement
			System.out.print("#if __I" + name +"_FORWARD >= 1\n");
			
			System.out.print("   return P" + name + "(");
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(cleanName(p.name));
				} else { 
					System.out.print(p.name);
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
			
			System.out.println(");");			
		
			System.out.println("#else");
			
			System.out.print("   IERROR(1, \"NOT IMPLEMENTED: " + name + " (");
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(p.type.name + " " + p.name + "=%p");
				} else { 
					System.out.print(p.type.name + " " + p.name + "=" + p.type.getFormat());
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}
		
			System.out.print(")\"");
			
			if (parameters.length > 0) { 
				System.out.print(", ");					
			}
			
			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
				
				if (p.pointer || p.array) { 
					System.out.print(cleanName(p.name));
				} else { 
					System.out.print(p.type.getValue(p.name));
				}
					
				if (i != parameters.length-1) { 
					System.out.print(", ");
				}
			}

			System.out.println(");");			
			System.out.println("   return MPI_ERR_INTERN;");
			
			System.out.println("#endif // __I" + name +"_FORWARD");
		}

		boolean canForward() {

			for (int i=0;i<parameters.length;i++) { 

				Parameter p = parameters[i];
		
				if (p.type.name.equals("MPI_Comm") ||
				    p.type.name.equals("MPI_Group") ||
					p.type.name.equals("MPI_Request")) { 
					return false;
				}
			}
			
			return true;
		}
		
		void generate(String what) { 

			if (what.equals("ibis")) { 
				System.out.println("#ifndef __I" + name);
				generateHeader(true, false);
				System.out.println("{");
				generateErrorCall();
				System.out.println("}");
				System.out.println("#endif // __I" + name);
				System.out.println("\n");
			
			} else if (what.equals("mpi")) {
				generateHeader(false, false);
				System.out.println("{");
				generateTrace();
				generateInterceptCall();
				System.out.println("}");
				System.out.println("\n");
				
			} else if (what.equals("header")) {
				
				if (canForward()) {
					System.out.println("#define __I" + name + "_FORWARD 1");
				} else { 
					System.out.println("#define __I" + name + "_FORWARD 0");
				}
				
				generateHeader(true, true);
				
				System.out.println();
			}
		}
	}
	
	private BufferedReader in;	
	private HashMap<String, Type> types = new HashMap<String, Type>();
	private LinkedList<Function> functions = new LinkedList<Function>();
	
	private Main(String file) {
		
		// Add all types that we know how to print.
		types.put("void", new Type("void", new ErrorPrinter()));
		types.put("char", new Type("char", new TypePrinter("%c")));
		types.put("int", new Type("int", new TypePrinter("%d")));
		types.put("double", new Type("double", new TypePrinter("%f")));
		types.put("MPI_Comm", new Type("MPI_Comm", new TypePrinter("%s", "comm_to_string")));
		types.put("MPI_Group", new Type("MPI_Group", new TypePrinter("%s", "group_to_string")));
		types.put("MPI_Request", new Type("MPI_Request", new TypePrinter("%s", "request_to_string")));
		types.put("MPI_Datatype", new Type("MPI_Datatype", new TypePrinter("%s", "type_to_string")));
		types.put("MPI_Op", new Type("MPI_Op", new TypePrinter("%s", "op_to_string")));
		types.put("MPI_Info", new Type("MPI_Info", new TypePrinter("%s", "info_to_string")));
		types.put("MPI_File", new Type("MPI_File", new TypePrinter("%s", "file_to_string")));
		types.put("MPI_Win", new Type("MPI_Win", new TypePrinter("%s", "win_to_string")));
		types.put("MPI_Fint", new Type("MPI_Fint", new TypePrinter("%d")));
		
		open(file);
	}
	
	private static void fatal(String error, Exception e) { 
		
		System.err.println(error);
		
		if (e != null) { 
			System.err.println(e);
			e.printStackTrace(System.err);
		}
		
		System.exit(1);	
	}
	
	private void open(String file) { 
		try {
			in = new BufferedReader(new FileReader(file));
		} catch (FileNotFoundException e) {
			fatal("Failed to open input file: " + file, e);
		}
	}

	private Type getType(String name) {
		
		Type type = types.get(name);
		
		if (type == null) {
			type = new Type(name, new UnknownTypePrinter());
			types.put(name, type);
		}
		
		return type;
	}

	private Parameter getParameter(String type, String name) { 
		
		boolean pointer = name.startsWith("*");
		boolean array = name.endsWith("]");		
		return new Parameter(getType(type), name, pointer, array);
	}	
	
	private void parse(String line) { 

		StringTokenizer tok = new StringTokenizer(line, " (),");
		
		Type ret = getType(tok.nextToken());

		String name = tok.nextToken();
		
		ArrayList<Parameter> params = new ArrayList<Parameter>();
		
		while (tok.hasMoreTokens()) { 
			params.add(getParameter(tok.nextToken(), tok.nextToken()));
		}
	
		functions.add(new Function(ret, name, params.toArray(new Parameter[params.size()])));
	}

	private String readLine() { 

		try {
			String line = in.readLine();

			if (line == null) { 
				return null;
			}

			line = line.trim();
			
			String tmp = in.readLine(); 
			
			while (tmp != null && !tmp.equals("#")) { 
				line += " " + tmp.trim();
				tmp = in.readLine();
			}
			
			return line;
		
		} catch (IOException e) {
			fatal("Parse error", e);
			return null; 
		}
	} 
	
	private void parse() { 
		
		String line = readLine();

		while (line != null) {
			parse(line);
			line = readLine();
		}		
	}
	
	private void print(String what) { 
		for (Function f : functions) { 
			f.generate(what);
		}
	}

	private void run(String what) { 
		parse();
		print(what);
	}
	
	public static void main(String [] args) { 
		
		if (args.length != 2) {
			fatal("Usage: generator.Main <inputfile> <what>", null);
			System.exit(1);
		}

		new Main(args[0]).run(args[1]);
	}
	
	
}
