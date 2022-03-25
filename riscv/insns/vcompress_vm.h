// vcompress vd, vs2, vs1
require(P.VU.vstart->read() == 0);
require_align(insn.rd(), P.VU.vflmul);
require_align(insn.rs2(), P.VU.vflmul);
require(insn.rd() != insn.rs2());
require_noover(insn.rd(), P.VU.vflmul, insn.rs1(), 1);

reg_t pos = 0;

VI_GENERAL_LOOP_BASE
  const int midx = i / 64;
  const int mpos = i % 64;

  bool do_mask = (P.VU.elt<uint64_t>(rs1_num, midx) >> mpos) & 0x1;
  if (do_mask) {
    switch (sew) {
    case e8:
      P.VU.elt<uint8_t>(rd_num, pos, true) = P.VU.elt<uint8_t>(rs2_num, i);
      break;
    case e16:
      P.VU.elt<uint16_t>(rd_num, pos, true) = P.VU.elt<uint16_t>(rs2_num, i);
      break;
    case e32:
      P.VU.elt<uint32_t>(rd_num, pos, true) = P.VU.elt<uint32_t>(rs2_num, i);
      break;
    default:
      P.VU.elt<uint64_t>(rd_num, pos, true) = P.VU.elt<uint64_t>(rs2_num, i);
      break;
    }

    ++pos;
  }
VI_LOOP_END
#ifdef RISCV_ENABLE_VECTOR_TAIL_AGNOSTIC_OVERWRITE
if (vta) {
  VI_AGNOSTIC_PARAM
  reg_t loop_max = (vl == 0) ? 0 : vlmax;
  for (reg_t i=pos; i<loop_max; ++i){
    if (sew == e8){
      VAGN_PARAMS(e8);
      vd = -1;
    }else if(sew == e16){
      VAGN_PARAMS(e16);
      vd = -1;
    }else if(sew == e32){
      VAGN_PARAMS(e32);
      vd = -1;
    }else if(sew == e64){
      VAGN_PARAMS(e64);
      vd = -1;
    }
  }
}
#endif
