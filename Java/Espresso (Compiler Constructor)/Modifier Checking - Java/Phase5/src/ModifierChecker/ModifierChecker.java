package ModifierChecker;

import java.util.HashMap;

import AST.*;
import Utilities.*;
import NameChecker.*;
import TypeChecker.*;
import Utilities.Error;

public class ModifierChecker extends Visitor {

	private SymbolTable classTable;
	private HashMap<ClassBodyDecl,ClassDecl> rrealClassTable;
	private ClassDecl currentClass;
	private ClassBodyDecl currentContext;
	private boolean leftHandSide = false;
	private AST tempContext; 

	public ModifierChecker(SymbolTable classTable, boolean debug) {
		this.classTable = classTable;
		this.debug = debug;
		this.rrealClassTable=new HashMap<>();
	}

	/** Assignment */
	public Object visitAssignment(Assignment as) {
	    println(as.line + ": Visiting an assignment (Operator: " + as.op()+ ")");

		boolean oldLeftHandSide = leftHandSide;

		leftHandSide = true;
		as.left().visit(this);

		// Added 06/28/2012 - no assigning to the 'length' field of an array type
		if (as.left() instanceof FieldRef) {
			FieldRef fr = (FieldRef)as.left();
			if (fr.target().type.isArrayType() && fr.fieldName().getname().equals("length"))
				Error.error(fr,"Cannot assign a value to final variable length.");
		}

		leftHandSide = oldLeftHandSide;
		as.right().visit(this);

		return null;
	}

	/** CInvocation */
	public Object visitCInvocation(CInvocation ci) {
	    println(ci.line + ": Visiting an explicit constructor invocatioon (" + (ci.superConstructorCall() ? "super" : "this") + ").");

		// YOUR CODE HERE
	    if (ci.args()!=null)
	    	ci.args().visit(this);
	    if (ci.targetClass!=currentClass && ci.constructor.getModifiers().isPrivate())
	    	Error.error(ci,"Private constructor invoked from other class.");
	    if (ci.superConstructorCall() && ci.constructor.getModifiers().isPrivate())
	    	Error.error(ci,"Private constructor cannot be called of Super class. ");

	    if (ci.args()!=null)
	    	ci.args().visit(this);	    	
		return null;
	}

	/** ClassDecl */
	public Object visitClassDecl(ClassDecl cd) {
		println(cd.line + ": Visiting a class declaration for class '" + cd.name() + "'.");

		currentClass = cd;

		// If this class has not yet been declared public make it so.
		if (!cd.modifiers.isPublic())
			cd.modifiers.set(true, false, new Modifier(Modifier.Public));

		// If this is an interface declare it abstract!
		if (cd.isInterface() && !cd.modifiers.isAbstract())
			cd.modifiers.set(false, false, new Modifier(Modifier.Abstract));

		// If this class extends another class then make sure it wasn't declared
		// final.
		if (cd.superClass() != null)
			if (cd.superClass().myDecl.modifiers.isFinal())
				Error.error(cd, "Class '" + cd.name()
						+ "' cannot inherit from final class '"
						+ cd.superClass().typeName() + "'.");

		// YOUR CODE HERE
		cd.modifiers().visit(this);
		ClassType superCT = cd.superClass();
		if (superCT != null && superCT.myDecl.getModifiers().isFinal())
			Error.error(cd,"Cannot Extend A final Class.");
		cd.body().visitChildren(this);
		

		return null;
	}

	/** FieldDecl */
	public Object visitFieldDecl(FieldDecl fd) {
	    println(fd.line + ": Visiting a field declaration for field '" +fd.var().name() + "'.");

		// If field is not private and hasn't been declared public make it so.
		if (!fd.modifiers.isPrivate() && !fd.modifiers.isPublic())
			fd.modifiers.set(false, false, new Modifier(Modifier.Public));

		// YOUR CODE HERE
		this.currentContext=fd;
		fd.var().visit(this);
		fd.modifiers().visit(this);
		this.leftHandSide=true;
		fd.var().visit(this);
		Modifiers fdm = fd.getModifiers();
		if (fdm.isFinal() && fd.var().init()==null)
			Error.error(fd,"Final Variable '" + fd.var().name() + "' not instantiated.");
		Type fdt = fd.type();
		if (fd.isClassType()){
			ClassType fdct = (ClassType)fdt;
			boolean CA = true;	//Public Constructor Available
			if (fdct.myDecl!=currentClass){
				CA=false;
				Sequence constructors = fdct.myDecl.constructors;
				for (int i =0;i<constructors.nchildren && !CA;++i){
					if (((ConstructorDecl)constructors.children[i]).getModifiers().isPublic())
						CA=true;
				}
			}
			if (!CA && !fdct.myDecl.modifiers.isAbstract())
				Error.error(fd,"No Public constructor available for the declared field.");
		}
		
		this.leftHandSide=false;
		this.rrealClassTable.put(fd, currentClass);

		return null;
	}

	/** FieldRef */
	public Object visitFieldRef(FieldRef fr) {
	    println(fr.line + ": Visiting a field reference '" + fr.fieldName() + "'.");

		// YOUR CODE HERE
	    
	    Type frt = (Type)fr.target().visit(this);
	    Modifiers frm = fr.myDecl.getModifiers();
	    
	    //MethodDecl inmd = in.targetMethod;
	    
	    if (frt!=null && frt.isClassType()){
	    	ClassType frct = (ClassType)frt;
	    	if (frm.isPrivate() && 
		    		currentClass!=rrealClassTable.get(
		    				this.rrealClassTable.get(fr.myDecl))){
	    		
		    	if (this.tempContext instanceof VarDecl )
		    		Error.error(fr,"Private Declared Field cannot be accessed from another class.");
		    }
	    	//this.leftHandSide=true;
	    }
	   if (currentContext.isStatic() && tempContext instanceof ClassDecl){
	    	if (!frm.isStatic())
	    		Error.error(fr,"Non Static Field reference from a static context.");
	    }
	    
	    if (leftHandSide==true && frm.isFinal())
	    	Error.error(fr,"Cannot re-assign the Final Field " + fr.fieldName().toString());
	    
	    this.tempContext=null;
	    
	    /*if (currentContext.isStatic()){
	    	if (!frm.isStatic())
	    		Error.error(fr,"Non Static Field reference from a static context.");
	    }
	    if (frm.isPrivate() && currentClass!=rrealClassTable.get(fr.myDecl)){
	    	Error.error(fr,"Private Declared Field cannot be accessed from another class.");
	    }*/

		return null;
	}

	/** MethodDecl */
	public Object visitMethodDecl(MethodDecl md) {
	    println(md.line + ": Visiting a method declaration for method '" + md.name() + "'.");

		// YOUR CODE HERE
	    
		//If method is not private and hasn't been declared public make it so.
		if (!md.getModifiers().isPrivate() && !md.getModifiers().isPublic())
			md.getModifiers().set(false, false, new Modifier(Modifier.Public));
		
		this.currentContext=md;
		md.params().visit(this);
		ClassDecl mdSuper = md.getMyClass().superClass().myDecl;
		Sequence sMethods = mdSuper.allMethods;
		for (int i =0;i<sMethods.nchildren;++i)
		{
			MethodDecl cMethod = (MethodDecl)sMethods.children[i];
			if (cMethod.name().toString().equals( md.name().toString())){
				if (cMethod.getModifiers().isFinal())
					Error.error(md,"Cannot override a Final Method from parent Class.");
				if (cMethod.isStatic() != md.isStatic())
					Error.error(md,"Overriding a method should have the same "
							+ "(non) static modifier.");
			}
		}
		
		sMethods = md.getMyClass().concreteMethods;
		for (int i =0;i<sMethods.nchildren;++i)
		{
			MethodDecl cMethod = (MethodDecl)sMethods.children[i];
			if (cMethod.name().equals(md.name()) && cMethod.isStatic()!=md.isStatic())
				Error.error(md,"Re-Declaration of a method should have the same "
						+ "(non) static modifier.");
		}
		this.rrealClassTable.put(md, currentClass);
		if (md.block()!=null)
			md.block().visitChildren(this);
		return null;
	}

	/** Invocation */
	public Object visitInvocation(Invocation in) {
	    println(in.line + ": Visiting an invocation of method '" + in.methodName() + "'.");

		// YOUR CODE HERE
	    MethodDecl inmd = in.targetMethod;
	    if (in.target()!=null){
	    	ClassType intype = (ClassType)in.target().visit(this);
		    if (inmd.getModifiers().isPrivate() && 
		    		currentClass!=rrealClassTable.get(in.targetMethod.getMyClass())){
		    	Error.error(in,"Private Declared Method cannot be accessed from another class.");
		    }
	    	//this.leftHandSide=true;
	    }
	    if (currentContext.isStatic()){
	    	if (!inmd.isStatic() &&  (tempContext == null ||
	    			tempContext instanceof ClassDecl))
	    		Error.error(in,"Non Static method reference from a static context.");
	    }
	    if (in.params()!= null)
	    	in.params().visit(this);
	    
	    this.tempContext=null;

	    //leftHandSide=false;
		return null;
	}


	public Object visitNameExpr(NameExpr ne) {
	    println(ne.line + ": Visiting a name expression '" + ne.name() + "'. (Nothing to do!)");
	    
	    if (ne.myDecl instanceof LocalDecl || ne.myDecl instanceof ParamDecl
	    		|| ne.myDecl instanceof FieldDecl) {
		    ne.type = ((VarDecl)ne.myDecl).type(); 
		}
		else if (ne.myDecl instanceof ClassDecl) {
		    // it wasn't a field - so it must be a class.
		    // if it weren't a class it would have been caught in the 
		    // name resolution phase
		    ne.type = new ClassType(((ClassDecl)ne.myDecl).className());
		    // or how about just new ClassType(ne.name()) Einstein!!!
		    ((ClassType)ne.type).myDecl = (ClassDecl)ne.myDecl;
		    
		} else 
		    Error.error(ne,"Unknown name expression '" + ne.name().getname() + "'.");
		//-->

	    this.tempContext=ne.myDecl;
		println(ne.line + ": Name Expression has type: " + ne.type);
		return ne.type;
	    //return null;
	}

	/** ConstructorDecl */
	public Object visitConstructorDecl(ConstructorDecl cd) {
	    println(cd.line + ": visiting a constructor declaration for class '" + cd.name() + "'.");

		// YOUR CODE HERE
	    this.currentContext=cd;
	    cd.params().visit(this);
	    Modifiers cdm = cd.getModifiers();
	    if (cdm.isFinal() || cdm.isStatic())
	    	Error.error(cd,"Constructor cannot be final or static.");
	    cd.body().visitChildren(this);
		return null;
	}

	/** New */
	public Object visitNew(New ne) {
	    println(ne.line + ": visiting a new '" + ne.type().myDecl.name() + "'.");

		// YOUR CODE HERE
		ConstructorDecl constructor;
		ne.type().visit(this);

		// Get the class of which we want to create a new object.
		ClassType ct = ne.type();
		ClassDecl cd = ct.myDecl;

		// We cannot create a new object based on an interface either.
		if (cd.isInterface())
		    Error.error(ne, "Cannot instantiate interface '" + cd.name() + "'.");

		// Generate the signature of the actual parameters
		Sequence actualParams = ne.args();
		Expression ap = null; // Holds the actual parameter.
		int actualParamCount = 0;
		String s = "";
		Type apt; // Holds the type of the actual parameter.

		if (actualParams != null)
		    actualParamCount = actualParams.nchildren;

		for (int i=0; i<actualParamCount; i++) {
		    ap = (Expression)actualParams.children[i];
		    apt = (Type)ap.visit(this);
		    //s = s + apt.signature();
		}

		// findMethod needs a sequence of methods to search, so make
		// the symboltable entry for <init> into a sequence

		constructor = (ConstructorDecl)TypeChecker.findMethod(cd.constructors, cd.name(), ne.args(), false);

		if (constructor == null) {
		    System.out.println("No constructor found for the pased Parameters..\nCandidates are:");
		    System.exit(1);
		}

		ne.setConstructorDecl(constructor);
		if (constructor.getModifiers().isPrivate() && currentClass !=cd)
			Error.error(ne,"Cannot invoke a private constructor through new.");
		ne.type = ct;
		//-->

		this.tempContext=ne.type;
		this.tempContext= new 
				LocalDecl(ct,new Var(new Name(new Token(12,"",ne.line,0,1))));
		//System.out.println(()this.tempContext);
		
		println(ne.line + ": New has type: " + ne.type);
	    return ne.type();
	    

		//return null;
	}

	/** StaticInit */
	public Object visitStaticInitDecl(StaticInitDecl si) {
		println(si.line + ": visiting a static initializer");

		// YOUR CODE HERE
		//Error.error(si,"Phara gya!!!");
		si.visitChildren(this);
		

		return null;
	}

	/** Super */
	public Object visitSuper(Super su) {
		println(su.line + ": visiting a super");

		if (currentContext.isStatic())
			Error.error(su,
					"non-static variable super cannot be referenced from a static context");

		return null;
	}

	/** This */
	public Object visitThis(This th) {
		println(th.line + ": visiting a this");

		if (currentContext.isStatic())
			Error.error(th,	"non-static variable this cannot be referenced from a static context");

		return null;
	}

	/** UnaryPostExpression */
    public Object visitUnaryPostExpr(UnaryPostExpr up) {
	println(up.line + ": visiting a unary post expression with operator '" + up.op() + "'.");
	
	// YOUR CODE HERE
	this.leftHandSide=true;
	up.expr().visit(this);
	this.leftHandSide=false;
	
	
	return null;
    }
    
    /** UnaryPreExpr */
    public Object visitUnaryPreExpr(UnaryPreExpr up) {
	println(up.line + ": visiting a unary pre expression with operator '" + up.op() + "'.");
	
	// YOUR CODE HERE
	this.leftHandSide=true;
	up.expr().visit(this);
	this.leftHandSide=false;
	return null;
    }
}
