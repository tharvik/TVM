opcode_macro(nop,0x00,1)
opcode_macro(aconst_null,0x01,1)
opcode_macro(iconst_m1,0x02,1)
//opcode_macro(iconst_0,0x03,1)
//opcode_macro(iconst_1,0x04,1)
//opcode_macro(iconst_2,0x05,1)
//opcode_macro(iconst_3,0x06,1)
//opcode_macro(iconst_4,0x07,1)
//opcode_macro(iconst_5,0x08,1)
opcode_macro(lconst_0,0x09,1)
opcode_macro(lconst_1,0x0A,1)
opcode_macro(fconst_0,0x0B,1)
opcode_macro(fconst_1,0x0C,1)
opcode_macro(fconst_2,0x0D,1)
opcode_macro(dconst_0,0x0E,1)
opcode_macro(dconst_1,0x0F,1)
//opcode_macro(bipush,0x10,2)
//opcode_macro(sipush,0x11,1)
//opcode_macro(ldc,0x12,2)
opcode_macro(ldc_w,0x13,1)
opcode_macro(ldc2_w,0x14,1)
//opcode_macro(iload,0x15,1)
opcode_macro(lload,0x16,1)
opcode_macro(fload,0x17,1)
opcode_macro(dload,0x18,1)
//opcode_macro(aload,0x19,2)
//opcode_macro(iload_0,0x1A,1)
//opcode_macro(iload_1,0x1B,1)
//opcode_macro(iload_2,0x1C,1)
//opcode_macro(iload_3,0x1D,1)
opcode_macro(lload_0,0x1E,1)
opcode_macro(lload_1,0x1F,1)
opcode_macro(lload_2,0x20,1)
opcode_macro(lload_3,0x21,1)
opcode_macro(fload_0,0x22,1)
opcode_macro(fload_1,0x23,1)
opcode_macro(fload_2,0x24,1)
opcode_macro(fload_3,0x25,1)
opcode_macro(dload_0,0x26,1)
opcode_macro(dload_1,0x27,1)
opcode_macro(dload_2,0x28,1)
opcode_macro(dload_3,0x29,1)
//opcode_macro(aload_0,0x2A,1)
//opcode_macro(aload_1,0x2B,1)
//opcode_macro(aload_2,0x2C,1)
//opcode_macro(aload_3,0x2D,1)
//opcode_macro(iaload,0x2E,1)
opcode_macro(laload,0x2F,1)
opcode_macro(faload,0x30,1)
opcode_macro(daload,0x31,1)
opcode_macro(aaload,0x32,1)
opcode_macro(baload,0x33,1)
opcode_macro(caload,0x34,1)
opcode_macro(saload,0x35,1)
//opcode_macro(istore,0x36,1)
opcode_macro(lstore,0x37,1)
opcode_macro(fstore,0x38,1)
opcode_macro(dstore,0x39,1)
//opcode_macro(astore,0x3A,1)
//opcode_macro(istore_0,0x3B,1)
//opcode_macro(istore_1,0x3C,1)
//opcode_macro(istore_2,0x3D,1)
//opcode_macro(istore_3,0x3E,1)
opcode_macro(lstore_0,0x3F,1)
opcode_macro(lstore_1,0x40,1)
opcode_macro(lstore_2,0x41,1)
opcode_macro(lstore_3,0x42,1)
opcode_macro(fstore_0,0x43,1)
opcode_macro(fstore_1,0x44,1)
opcode_macro(fstore_2,0x45,1)
opcode_macro(fstore_3,0x46,1)
opcode_macro(dstore_0,0x47,1)
opcode_macro(dstore_1,0x48,1)
opcode_macro(dstore_2,0x49,1)
opcode_macro(dstore_3,0x4A,1)
//opcode_macro(astore_0,0x4B,1)
//opcode_macro(astore_1,0x4C,1)
//opcode_macro(astore_2,0x4D,1)
//opcode_macro(astore_3,0x4E,1)
//opcode_macro(iastore,0x4F,1)
opcode_macro(lastore,0x50,1)
opcode_macro(fastore,0x51,1)
opcode_macro(dastore,0x52,1)
opcode_macro(aastore,0x53,1)
opcode_macro(bastore,0x54,1)
opcode_macro(castore,0x55,1)
opcode_macro(sastore,0x56,1)
//opcode_macro(pop,0x57,1)
opcode_macro(pop2,0x58,1)
//opcode_macro(dup,0x59,1)
opcode_macro(dup_x1,0x5A,1)
opcode_macro(dup_x2,0x5B,1)
opcode_macro(dup2,0x5C,1)
opcode_macro(dup2_x1,0x5D,1)
opcode_macro(dup2_x2,0x5E,1)
opcode_macro(swap,0x5F,1)
//opcode_macro(iadd,0x60,1)
opcode_macro(ladd,0x61,1)
opcode_macro(fadd,0x62,1)
opcode_macro(dadd,0x63,1)
//opcode_macro(isub,0x64,1)
opcode_macro(lsub,0x65,1)
opcode_macro(fsub,0x66,1)
opcode_macro(dsub,0x67,1)
//opcode_macro(imul,0x68,1)
opcode_macro(lmul,0x69,1)
opcode_macro(fmul,0x6A,1)
opcode_macro(dmul,0x6B,1)
//opcode_macro(idiv,0x6C,1)
opcode_macro(ldiv,0x6D,1)
opcode_macro(fdiv,0x6E,1)
opcode_macro(ddiv,0x6F,1)
opcode_macro(irem,0x70,1)
opcode_macro(lrem,0x71,1)
opcode_macro(frem,0x72,1)
opcode_macro(drem,0x73,1)
opcode_macro(ineg,0x74,1)
opcode_macro(lneg,0x75,1)
opcode_macro(fneg,0x76,1)
opcode_macro(dneg,0x77,1)
opcode_macro(ishl,0x78,1)
opcode_macro(lshl,0x79,1)
opcode_macro(ishr,0x7A,1)
opcode_macro(lshr,0x7B,1)
opcode_macro(iushr,0x7C,1)
opcode_macro(lushr,0x7D,1)
opcode_macro(iand,0x7E,1)
opcode_macro(land,0x7F,1)
opcode_macro(ior,0x80,1)
opcode_macro(lor,0x81,1)
opcode_macro(ixor,0x82,1)
opcode_macro(lxor,0x83,1)
opcode_macro(iinc,0x84,1)
opcode_macro(i2l,0x85,1)
opcode_macro(i2f,0x86,1)
opcode_macro(i2d,0x87,1)
opcode_macro(l2i,0x88,1)
opcode_macro(l2f,0x89,1)
opcode_macro(l2d,0x8A,1)
opcode_macro(f2i,0x8B,1)
opcode_macro(f2l,0x8C,1)
opcode_macro(f2d,0x8D,1)
opcode_macro(d2i,0x8E,1)
opcode_macro(d2l,0x8F,1)
opcode_macro(d2f,0x90,1)
opcode_macro(i2b,0x91,1)
opcode_macro(i2c,0x92,1)
opcode_macro(i2s,0x93,1)
opcode_macro(lcmp,0x94,1)
opcode_macro(fcmpl,0x95,1)
opcode_macro(fcmpg,0x96,1)
opcode_macro(dcmpl,0x97,1)
opcode_macro(dcmpg,0x98,1)
//opcode_macro(ifeq,0x99,3)
opcode_macro(ifne,0x9A,3)
opcode_macro(iflt,0x9B,3)
opcode_macro(ifge,0x9C,3)
opcode_macro(ifgt,0x9D,3)
opcode_macro(ifle,0x9E,3)
//opcode_macro(if_icmpeq,0x9F,3)
opcode_macro(if_icmpne,0xA0,3)
//opcode_macro(if_icmplt,0xA1,3)
opcode_macro(if_icmpge,0xA2,3)
opcode_macro(if_icmpgt,0xA3,3)
opcode_macro(if_icmple,0xA4,3)
opcode_macro(if_acmpeq,0xA5,3)
opcode_macro(if_acmpne,0xA6,3)
//opcode_macro(op_goto,0xA7,1)
opcode_macro(jsr,0xA8,1)
opcode_macro(ret,0xA9,1)
opcode_macro(tableswitch,0xAA,1)
opcode_macro(lookupswitch,0xAB,1)
//opcode_macro(ireturn,0xAC,1)
opcode_macro(lreturn,0xAD,1)
opcode_macro(freturn,0xAE,1)
opcode_macro(dreturn,0xAF,1)
//opcode_macro(areturn,0xB0,1)
//opcode_macro(op_return,0xB1,1)
//opcode_macro(getstatic,0xB2,3)
opcode_macro(putstatic,0xB3,1)
//opcode_macro(getfield,0xB4,1)
//opcode_macro(putfield,0xB5,1)
//opcode_macro(invokevirtual,0xB6,3)
//opcode_macro(invokespecial,0xB7,3)
opcode_macro(invokestatic,0xB8,1)
opcode_macro(invokeinterface,0xB9,1)
opcode_macro(unused,0xBA,1)
//opcode_macro(op_new,0xBB,3)
//opcode_macro(newarray,0xBC,1)
opcode_macro(anewarray,0xBD,1)
opcode_macro(arraylength,0xBE,1)
opcode_macro(athrow,0xBF,1)
opcode_macro(checkcast,0xC0,1)
opcode_macro(instanceof,0xC1,1)
opcode_macro(monitorenter,0xC2,1)
opcode_macro(monitorexit,0xC3,1)
opcode_macro(wide,0xC4,1)
opcode_macro(multianewarray,0xC5,1)
opcode_macro(ifnull,0xC6,1)
opcode_macro(ifnonnull,0xC7,1)
opcode_macro(goto_w,0xC8,1)
opcode_macro(jsr_w,0xC9,1)
opcode_macro(breakpoint,0xCA,1)
opcode_macro(impdep1,0xFE,1)
opcode_macro(impdep2,0xFF,1)
