#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define strkeq(s1,s2,k) ( k<=0 ? 1 : s1[0]==s2[0] ? !strncmp(s1,s2,(k)) : 0)
#define column(n)      (linebuf+(n)-1)


/*#define TRACE*/

#define UNKNOWN -1
/***** PDB file van der Waals Radii Definitions *****/
/***** Small adjustments made by L Kuhn, 7/31/07, to match *****/
/***** data in Table XI: The Minimum Radii of Atoms, in Li & Nussinov (1998)
       "A Set of van der Waals and Coulombic Radii of Protein Atoms for Molecular
       and Solvent-Accessible Surface Calculation, Packing Evaluation, and Docking",
       Proteins 21, 111-127. *****/

/***** Instructions to add a new atom type *****/
/* Add a radius definition below, then add the atom type
   assignment in read_pdb.c (see that file at 'ADD ATOM'),
   and add the atom name definition in inc/defs.h (also at
   'ADD ATOM' in that file */

#define RAD_ACEO    1.26
#define RAD_ACEC    1.62
#define RAD_ACECA   1.73
#define RAD_ALAN    1.43
#define RAD_ALACA   1.73
#define RAD_ALAC    1.62
#define RAD_ALAO    1.26
#define RAD_ALAOXT  1.24
#define RAD_ALACB   1.66
#define RAD_ARGN    1.43
#define RAD_ARGCA   1.73
#define RAD_ARGC    1.62
#define RAD_ARGO    1.26
#define RAD_ARGOXT  1.24
#define RAD_ARGCB   1.69
#define RAD_ARGCG   1.68
#define RAD_ARGCD   1.62
#define RAD_ARGNE   1.34
#define RAD_ARGCZ   1.62
#define RAD_ARGNH1  1.34
#define RAD_ARGNH2  1.34
#define RAD_ASNN    1.43
#define RAD_ASNCA   1.73
#define RAD_ASNC    1.62
#define RAD_ASNO    1.26
#define RAD_ASNOXT  1.24
#define RAD_ASNCB   1.69
#define RAD_ASNCG   1.67
#define RAD_ASNOD1  1.20
#define RAD_ASNND2  1.31
#define RAD_ASNAD1  1.25   /* average of OD1 and ND2 */
#define RAD_ASNAD2  1.25   /* average of OD1 and ND2 */
#define RAD_ASPN    1.43
#define RAD_ASPCA   1.73
#define RAD_ASPC    1.62
#define RAD_ASPO    1.26
#define RAD_ASPOXT  1.24
#define RAD_ASPCB   1.69
#define RAD_ASPCG   1.65
#define RAD_ASPOD1  1.24
#define RAD_ASPOD2  1.24
#define RAD_CYSN    1.43
#define RAD_CYSCA   1.73
#define RAD_CYSC    1.62
#define RAD_CYSO    1.26
#define RAD_CYSOXT  1.24
#define RAD_CYSCB   1.69
#define RAD_CYSSG   1.54
#define RAD_GLNN    1.43
#define RAD_GLNCA   1.73
#define RAD_GLNC    1.62
#define RAD_GLNO    1.26
#define RAD_GLNOXT  1.24
#define RAD_GLNCB   1.69
#define RAD_GLNCG   1.62
#define RAD_GLNCD   1.67
#define RAD_GLNOE1  1.20
#define RAD_GLNNE2  1.31
#define RAD_GLNAE1  1.25   /* average of OE1 and NE2 */
#define RAD_GLNAE2  1.25   /* average of OE1 and NE2 */
#define RAD_GLUN    1.43
#define RAD_GLUCA   1.73
#define RAD_GLUC    1.62
#define RAD_GLUO    1.26
#define RAD_GLUOXT  1.24
#define RAD_GLUCB   1.69
#define RAD_GLUCG   1.62
#define RAD_GLUCD   1.65
#define RAD_GLUOE1  1.24
#define RAD_GLUOE2  1.24
#define RAD_GLYN    1.43
#define RAD_GLYCA   1.68   /* CH2 atom type in Li & Nussinov */
#define RAD_GLYC    1.62
#define RAD_GLYO    1.26
#define RAD_GLYOXT  1.24
#define RAD_HISN    1.43
#define RAD_HISCA   1.73
#define RAD_HISC    1.62
#define RAD_HISO    1.26
#define RAD_HISOXT  1.24
#define RAD_HISCB   1.69
#define RAD_HISCG   1.62
#define RAD_HISND1  1.38
#define RAD_HISCD2  1.50
#define RAD_HISCE1  1.50
#define RAD_HISNE2  1.38
#define RAD_ILEN    1.43
#define RAD_ILECA   1.73
#define RAD_ILEC    1.62
#define RAD_ILEO    1.26
#define RAD_ILEOXT  1.24
#define RAD_ILECB   1.82
#define RAD_ILECG1  1.68
#define RAD_ILECG2  1.66
#define RAD_ILECD1  1.66
#define RAD_LEUN    1.43
#define RAD_LEUCA   1.73
#define RAD_LEUC    1.62
#define RAD_LEUO    1.26
#define RAD_LEUOXT  1.24
#define RAD_LEUCB   1.69
#define RAD_LEUCG   1.82
#define RAD_LEUCD1  1.66
#define RAD_LEUCD2  1.66
#define RAD_LYSN    1.43
#define RAD_LYSCA   1.73
#define RAD_LYSC    1.62
#define RAD_LYSO    1.26
#define RAD_LYSOXT  1.24
#define RAD_LYSCB   1.69
#define RAD_LYSCG   1.68
#define RAD_LYSCD   1.68
#define RAD_LYSCE   1.62
#define RAD_LYSNZ   1.22
#define RAD_METN    1.43
#define RAD_METCA   1.73
#define RAD_METC    1.62
#define RAD_METO    1.26
#define RAD_METOXT  1.24
#define RAD_METCB   1.69
#define RAD_METCG   1.68
#define RAD_METSD   1.67
#define RAD_METCE   1.66
#define RAD_PHEN    1.43
#define RAD_PHECA   1.73
#define RAD_PHEC    1.62
#define RAD_PHEO    1.26
#define RAD_PHEOXT  1.24
#define RAD_PHECB   1.69
#define RAD_PHECG   1.62
#define RAD_PHECD1  1.62
#define RAD_PHECD2  1.62
#define RAD_PHECE1  1.62
#define RAD_PHECE2  1.62
#define RAD_PHECZ   1.62
#define RAD_PRON    1.43
#define RAD_PROCA   1.73
#define RAD_PROC    1.62
#define RAD_PROO    1.26
#define RAD_PROOXT  1.24
#define RAD_PROCB   1.69
#define RAD_PROCG   1.68
#define RAD_PROCD   1.68
#define RAD_SERN    1.43
#define RAD_SERCA   1.73
#define RAD_SERC    1.62
#define RAD_SERO    1.26
#define RAD_SEROXT  1.24
#define RAD_SERCB   1.69
#define RAD_SEROG   1.30
#define RAD_THRN    1.43
#define RAD_THRCA   1.73
#define RAD_THRC    1.62
#define RAD_THRO    1.26
#define RAD_THROXT  1.24
#define RAD_THRCB   1.82
#define RAD_THROG1  1.30
#define RAD_THRCG2  1.66
#define RAD_TRPN    1.43
#define RAD_TRPCA   1.73
#define RAD_TRPC    1.62
#define RAD_TRPO    1.26
#define RAD_TRPOXT  1.24
#define RAD_TRPCB   1.69
#define RAD_TRPCG   1.62
#define RAD_TRPCD1  1.62
#define RAD_TRPCD2  1.62
#define RAD_TRPNE1  1.35
#define RAD_TRPCE2  1.62
#define RAD_TRPCE3  1.62
#define RAD_TRPCZ2  1.62
#define RAD_TRPCZ3  1.62
#define RAD_TRPCH2  1.62
#define RAD_TYRN    1.43
#define RAD_TYRCA   1.73
#define RAD_TYRC    1.62
#define RAD_TYRO    1.26
#define RAD_TYROXT  1.24
#define RAD_TYRCB   1.69
#define RAD_TYRCG   1.62
#define RAD_TYRCD1  1.62
#define RAD_TYRCD2  1.62
#define RAD_TYRCE1  1.62
#define RAD_TYRCE2  1.62
#define RAD_TYRCZ   1.62
#define RAD_TYROH   1.30
#define RAD_VALN    1.43
#define RAD_VALCA   1.73
#define RAD_VALC    1.62
#define RAD_VALO    1.26
#define RAD_VALOXT  1.24
#define RAD_VALCB   1.82
#define RAD_VALCG1  1.66
#define RAD_VALCG2  1.66
#define RAD_H       1.00 /* Not from Li & Nussinov */

/***** MOL2 file van der Waals Radii Definitions *****/

/***** Instructions to add a new atom type *****/
/* Add a radius definition below, then add the atom type
   assignment in assign_type.c (see that file at 'ADD MOL2 ATOM'),
   add the atom radius assignment code in read_mol2.c (at 
   'ADD MOL2 ATOM RAD'), and add the atom name definition in 
   inc/defs.h (at 'ADD MOL2 ATOM') */


#define MRAD_AG       1.550
#define MRAD_AL       1.170  /* Batsanov et al has this value as 1.8 20MAR08 toneroma - previous value 1.500, Amber/Dock5 has this value as 1.17 */
#define MRAD_AS       1.700  /* Batsanov et al 20MAR08 toneroma - previous value 0.830*/
#define MRAD_AU       1.900  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_B        1.700  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_BA       3.100  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_BE       1.900  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_BI       1.800  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_BR       1.950  /* Batsanov et al has this value as 1.9  20MAR08 toneroma */
#define MRAD_CSP1     1.850
#define MRAD_CSP2     1.850
#define MRAD_CSP3     1.800
#define MRAD_CCAT     1.800
#define MRAD_CAR      1.800 
#define MRAD_CDEF     1.850
#define MRAD_CA       1.600  /* Batsanov et alhas this value as 2.5 20MAR08 toneroma */
#define MRAD_CD       2.300  /* Batsanov et al 20MAR08 toneroma - previous value 1.750 */
#define MRAD_CE       1.860
#define MRAD_CL       2.030  /* Batsanov et al has this value as 1.8  20MAR08 toneroma */
#define MRAD_CO       1.130
#define MRAD_CR       1.130
#define MRAD_CS       3.500  /* Batsanov et al 20MAR08 toneroma - previous value 3.010 */
#define MRAD_CU       1.900  /* Batsanov et al 20MAR08 toneroma - previous value 1.150 */
#define MRAD_ER       1.590
#define MRAD_F        1.550  /* Batsanov et al has this value as 1.5 20MAR08 toneroma */
#define MRAD_FE       1.950
#define MRAD_GA       1.550
#define MRAD_GE       2.000  /* Batsanov et al 20MAR08 toneroma - previous value 2.720 */
#define MRAD_H        1.000
#define MRAD_HG       2.000  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_I        2.350  /* Batsanov et al has this value as 2.1 20MAR08 toneroma */
#define MRAD_IN       1.900  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_K        1.600  /* Batsanov et al has this value as 3.0 20MAR08 toneromav.  The previous value in slide was 2.390, DOCK has the value as 1.600 */
#define MRAD_LA       1.830
#define MRAD_LI       1.170  /* Batsanov et al has this value as 2.6 20MAR08 toneroma - previous value 1.220, Amber/Dock5 has this value as 1.17 */
#define MRAD_MG       1.170  /* Batsanov et al has this value as 2.2 20MAR08 toneroma - previous value 1.500, Amber/Dock5 has this value as 1.17 */
#define MRAD_MN       1.190
#define MRAD_MO       1.750
#define MRAD_NSP3     1.850
#define MRAD_NSP4     1.850
#define MRAD_NDEF     1.750
#define MRAD_NA       1.600  /* Batsanov et al has this value as 2.8 20MAR08 toneroma */
#define MRAD_NI       1.240
#define MRAD_OSP3     1.650
#define MRAD_ODEF     1.600
#define MRAD_OS       2.300  /* Batsanov et al 20MAR08 toneroma - previous value 1.580 */
#define MRAD_P        2.100  /* Batsanov et al has this value as 1.7 20MAR08 toneroma */
#define MRAD_PD       1.440
#define MRAD_RB       3.200  /* Batsanov et al 20MAR08 toneroma - previous value 2.650 */
#define MRAD_RE       1.300
#define MRAD_RH       1.220
#define MRAD_RU       1.200
#define MRAD_S        2.000  /* Batsanov et al has this value as 1.8 20MAR08 toneroma */
#define MRAD_SB       1.750  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_SE       1.850  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_SI       1.900  /* Batsanov et al 20MAR08 toneroma - previous value 2.200 */
#define MRAD_SN       2.300  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_SR       2.700  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_TE       2.000  /* Batsanov et al 20MAR08 toneroma - previous value none */
#define MRAD_TI       1.950
#define MRAD_TL       2.200  /* Batsanov et al 20MAR08 toneroma - previous value 1.710 */
#define MRAD_U        1.750
#define MRAD_V        1.060
#define MRAD_Y_       1.610
#define MRAD_ZN       2.200  /* Batsanov et al 20MAR08 toneroma - previous value 1.150 */
#define MRAD_ZR       1.420
#define MRAD_UNKNOWN  2.000

/* end define vdw radii */

/* other defs */
/* atom levels for side-chain flexibility */
#define  ALPHA    0
#define  BETA     1
#define  GAMMA    2
#define  DELTA    3
#define  EPSILON  4
#define  ZETA     5
#define  ETA      6

/* interaction types */
#define  NOTHING      16384 /* Changed from 0 to 1024 since sum-conflict of NOTHING + DONOR_HYDROGEN = DONEPTOR_DONEPTOR - Sameer 18/Feb/05*/
#define  ACCEPTOR          1
#define  DONOR             4
#define  HYDROPHOB        16
#define  DONEPTOR         64
#define  DONOR_HYDROGEN  256
#define  METAL_1        1024
#define  METAL_2        4096 

/* indices for coordinate vector */
#define  X            0
#define  Y            1
#define  Z            2

/* all atom types */
/* To add a new PDB atom type, add a line below (with the next number),
   add a radius definition in inc/vdwrad.h and add the parsing code to read_pdb.c
   (instructions at 'ADD ATOM') */
/* To add a new MOL2 atom type, add a line below (with the next number),
   add a radius definition in inc/vdwrad.h, add the type parsing code to
   assign_type.c (instructions at 'ADD MOL2 ATOM'), and add the radius
   assignment code to read_mol2.c (also at 'ADD MOL2 ATOM') */
#define  CA        1
#define  CB        2
#define  C         3
#define  O         4
#define  N         5
#define  CD        6
#define  CD1       7
#define  CD2       8
#define  CE        9
#define  CE1      10
#define  CE2      11
#define  CE3      12
#define  CG       13
#define  CG1      14
#define  CG2      15
#define  CH2      16
#define  CZ       17
#define  CZ2      18
#define  CZ3      19
#define  ND1      20
#define  ND2      21
#define  NE       22
#define  NE1      23
#define  NE2      24
#define  NH1      25
#define  NH2      26
#define  NZ       27
#define  OD1      28
#define  OD2      29
#define  OE1      30
#define  OE2      31
#define  OG       32
#define  OG1      33
#define  OH       34
#define  OXT      35
#define  SD       36
#define  SG       37
#define  S        40
#define  P        41
#define  H        42
#define  SI       43
#define  CO       44    /* metal type 2, hbond distance 2.6 */
#define  AG       45
#define  NI       46    /* metal type 1, hbond distance 2.0 */
#define  BR       47
#define  TL       48
#define  OS       49
#define  CL       50
#define  LA       51
#define  K        52    /* metal type 3, hbond distance 2.9 */
#define  FE       53    /* metal type 2, hbond distance 2.6 */
#define  DUMMY    54    /* dummy atom */
#define  MO       55
#define  I        56
#define  AS       57
#define  U        58
#define  F        59
#define  RE       60
#define  RU       61
#define  ER       62
#define  LI       63
#define  MN       64    /* metal type 2, hbond distance 2.6 */
#define  PD       65
#define  CU       66    /* metal type 2, hbond distance 2.6 */
#define  AL       67
#define  MG       68    /* metal type 2, hbond distance 2.6 */
#define  NA       69    /* metal type 3, hbond distance 2.9 */
#define  CR       70
#define  CS       71
#define  TI       72
#define  ZN       73    /* metal type 2, hbond distance 2.6 */
#define  RB       74
#define  V        75
#define  RH       76
#define  GE       77
#define  ZR       78
#define  GA       79
#define  Y_       80
#define  CA_M     82    /* metal type 2, this is metal "CA", not alpha carbon */

/***** Added by PCS -- 22-Mar-00 *****/
#define  OC       83  /* Too Close Oxygen - Very small radius */
/* added by Litian He 03/17/2004 */
#define  AD1      84   /* we have these 2 because the crystallographic can't decide O/N in ASN */
#define  AD2      85
#define  AE1      86   /* we have these 2 because the crystallographic can't decide O/N in GLN */
#define  AE2      87

/*************************************/
#define  HETATM  999
#define  WATER  1000

#define  ADDED     0
#define  SP1       1    /*  .1   */
#define  SP2       2    /*  .2   */
#define  SP3       3    /*  .3   */
/*       O         4        .o   */
#define  SP4       5    /*  .4   */
#define  AR        6    /*  .ar  */
#define  CO2       7    /*  .co2 */
#define  AM        8    /*  .am  */
#define  PL3       9    /*  .pl3 */
#define  CAT      10    /*  .cat */
#define  O2       11    /*  .o2  */
#define  TH       12    /*  .th  */
#define  AMBIG    13    /*  .ambiguous  */
#define  HISN     14    /*  .HIS-nitrogen  */
/*       OH       34        .oh  */

/* all residues */
#define  ALA      0
#define  ARG      1
#define  ASN      2
#define  ASP      3
#define  CYS      4
#define  GLN      5
#define  GLU      6
#define  GLY      7
#define  HIS      8
#define  ILE      9
#define  LEU     10
#define  LYS     11
#define  MET     12
#define  PHE     13
#define  PRO     14
#define  SER     15
#define  THR     16
#define  TRP     17
#define  TYR     18
#define  VAL     19
#define  ACE     20
#define  PCA     21

#define  HOH     22







/* atom entry in mol2 or pdb file */
typedef struct {
  /* components for all atom types */
  char          name[5];                            /* atom name in the file */
  float         pos[3];                           /* coordinates of the atom */
  float         rad;                                 /* van der Waals radius */
  float         charge;                      /* partial charge for this atom */
  float         charge_sum;
  int           atom_number; /* the number assigned to this atom in the file */
  int           type;                                    /* type of the atom */
  int           act;                   /* DONOR, ACCEPTOR, DONEPTOR, NOTHING */
  int           *neighbors;  /* indices of neighbors for intramolecular bump */
  float         *neighbor_dist;
  int           hydro;                               /* hydrophilicity value */
  /* components only relevant for mol2 atoms */
  char          type_str[7];                /* string defining the atom type */
  int           orbit;            /* orbit or whatever follows the . in type */
  int           number;    /* number assigned to each atom for fragmentation */
  int           fragment;  /* number of the fragment this atom is located in */
  /** Added by RSK - Aug 13, 2002 **/
  int           subst_id;                                 /* substructure id */
  char          subst_name[5];      /* substructure name */
  /* components only relevant for PDB atoms */
  int           level;                        /* level of atom in side-chain */
  int           residue_index; /* index of the residue in the residue vector */
  int           res;                          /* the residue type identifier */
  char          residue[4];                                  /* residue type */
  char          alt_location;               /* alternate location identifier */
  char          chain_id;                                /* chain identifier */
  char          insertion_code;                            /* insertion code */
  char          residue_num[5];                            /* residue number */
  /* components only relevant for waters */
  int                target_atom_index;  /* atom it is connected to (H-bond) */
  int                state;                        /* CONSERVED or DISPLACED */
  float              prediction;           /* the likelyhood of conservation */
} atom_t;

typedef atom_t  *atom_pt;


void  *mymalloc ( size_t  size )
{
  void  *adr;
  adr = malloc ( size );
  if ( !adr )
    { 
      fprintf( stderr, "malloc failed" );
      exit (EXIT_FAILURE);
    } 
  return ( adr );
}



float radius (int  type, int  orbit)
{
  float rad = -1.0;

  switch ( type ) {
    case AL:
      rad = MRAD_AL;
      break;
    case AG:
      rad = MRAD_AG;
      break;
    case AS:
      rad = MRAD_AS;
      break;
    case BR:
      rad = MRAD_BR;
      break;
    case C:
     rad = MRAD_CDEF;
     break;
    case CA:
      rad = MRAD_CA;
      break;
    case CD:
      rad = MRAD_CD;
      break;
    case CE:
      rad = MRAD_CE;
      break;
    case CL:
      rad = MRAD_CL;
      break;
    case CO:
      rad = MRAD_CO;
      break;
    case CR:
      rad = MRAD_CR;
      break;
    case CS:
      rad = MRAD_CS;
      break;
    case CU:
      rad = MRAD_CU;
      break;
    case ER:
      rad = MRAD_ER;
      break;
    case F: 
      rad = MRAD_F;
      break;
    case FE:
      rad = MRAD_FE;
      break;
    case GA:
      rad = MRAD_GA;
      break;
    case GE:
      rad = MRAD_GE;
      break;
    case H: 
      rad = MRAD_H;
      break;
    case I: 
      rad = MRAD_I;
      break;
    case K: 
      rad = MRAD_K;
      break;
    case LA:
      rad = MRAD_LA;
      break;
    case LI:
      rad = MRAD_LI;
      break;
    case MG:
      rad = MRAD_MG;
      break;
    case MN:
      rad = MRAD_MN;
      break;
    case MO:
      rad = MRAD_MO;
      break;
    case N:
      rad = MRAD_NDEF;
      break;
    case NA:
      rad = MRAD_NA;
      break;
    case NI:
      rad = MRAD_NI;
      break;
    case O:
      rad = MRAD_ODEF;
      break;
    case OS:
      rad = MRAD_OS;
      break;
    case P:
      rad = MRAD_P;
      break;
    case PD:
      rad = MRAD_PD;
      break;
    case RB:
      rad = MRAD_RB;
      break;
    case RE:
      rad = MRAD_RE;
      break;
    case RH:
      rad = MRAD_RH;
      break;
    case RU:
      rad = MRAD_RU; 
      break;
    case S:
      rad = MRAD_S;
      break; 
    case SI:
      rad = MRAD_SI;
      break;
    case TI:
      rad = MRAD_TI;
      break;
    case TL:
      rad = MRAD_TL;;
      break;
    case U:
      rad = MRAD_U;
      break;
    case V:
      rad = MRAD_V;
      break; 
    case Y_:
      rad = MRAD_Y_;
      break;
    case ZN:
      rad = MRAD_ZN;
      break;
    case ZR:
      rad = MRAD_ZR;
      break;
    case UNKNOWN:
      rad = MRAD_UNKNOWN;
      break;
    default:
      break;
    }
  return rad;
}




void  assign_residue_type ( atom_pt atm )
{
  switch ( atm->residue[0] )
    {
    case 'A':
      switch ( atm->residue[1] )
	{
	case 'C':
	  if ( strcmp ( atm->residue, "ACE" ) == 0 )
	    atm->res = ACE;  /*added by Litian   03/17/2004 */
	  break;
	case 'L':
	  atm->res = ALA;
	  break;
	case 'R':
	  atm->res = ARG;
	  break;
	case 'S':
	  switch ( atm->residue[2] )
	    {
	    case 'N':
	      atm->res = ASN;
	      break;
	    case 'P':
	      atm->res = ASP;
	      break;
	    default:
	      {
		fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
		exit (EXIT_FAILURE);
	      }
	      break;
	    }
	  break;
	default:
	  {
	    fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	    exit (EXIT_FAILURE);
	  }
	  break;
	}
      break;
    case 'C':
      if ( strncmp ( atm->residue, "CYS", 3 ) == 0 )
	atm->res = CYS;
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    case 'G':
      if ( atm->residue[1] == 'L' )
	switch ( atm->residue[2] )
	  {
	  case 'N':
	    atm->res = GLN;
	    break;
	  case 'U':
	    atm->res = GLU;
	    break;
	  case 'Y':
	    atm->res = GLY;
	    break;
	  default:
	    {
	      fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	      exit (EXIT_FAILURE);
	    }
	    break;
	  }
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    case 'H':
      if (( strncmp ( atm->residue, "HIS", 3 ) == 0 ) || ( strncmp ( atm->residue, "HID", 3 ) == 0 ) || ( strncmp ( atm->residue, "HIE", 3 ) == 0 ))
	atm->res = HIS;
      else
	if ( strncmp ( atm->residue, "HOH", 3 ) == 0 )
	  atm->res = HOH;
	else
	  {
	    fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	    exit (EXIT_FAILURE);
	  }
      break;
    case 'I':
      if ( strncmp ( atm->residue, "ILE", 3 ) == 0 )
	atm->res = ILE;
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    case 'L':
      switch ( atm->residue[1] )
	{
	case 'E':
	  atm->res = LEU;
	  break;
	case 'Y':
	  atm->res = LYS;
	  break;
	default:
	  {
	    fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	    exit (EXIT_FAILURE);
	  }
	  break;
	}
      break;
    case 'M':
      if ( strncmp ( atm->residue, "MET", 3 ) == 0 )
	atm->res = MET;
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    case 'P':
      switch ( atm->residue[1] )
	{
	case 'H':
	  atm->res = PHE;
	  break;
	case 'R':
	  atm->res = PRO;
	  break;
	case 'C':           /* treat PCA (pyrrolidone carboxylic acid) like Proline */
	  atm->res = PCA;
	  break;
	default:
	  {
	    fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	    exit (EXIT_FAILURE);
	  }
	  break;
	}
      break;
    case 'S':
      if ( strncmp ( atm->residue, "SER", 3 ) == 0 )
	atm->res = SER;
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    case 'T':
      switch ( atm->residue[1] )
	{
	case 'H':
	  atm->res = THR;
	  break;
	case 'R':
	  atm->res = TRP;
	  break;
	case 'Y':
	  atm->res = TYR;
	  break;
	default:
	  {
	    fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	    exit (EXIT_FAILURE);
	  }
	  break;
	}
      break;
    case 'V':
      if ( strncmp ( atm->residue, "VAL", 3 ) == 0 )
	atm->res = VAL;
      else
	{
	  fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	  exit (EXIT_FAILURE);
	}
      break;
    default:
      {
	fprintf(stderr, "Unknown Residue: %s %s\n", atm->residue, atm->residue_num);
	exit (EXIT_FAILURE);
      }
      break;
    }
}



void  assign_type_and_radius ( atom_pt atm)
{
  atm->level = ALPHA;     /* default level */
  atm->rad = -99.9;
  atm->act = NOTHING;
  if ( ( strncmp ( atm->name, "N", 1 ) == 0 
	 && atm->res != PRO && atm->res != PCA )
       || strncmp ( atm->name, "O", 1 ) == 0 ) 
    { 
      if ( strncmp ( atm->name, "N", 1 ) == 0 ) 
	atm->act = DONOR;
      if ( strncmp ( atm->name, "O", 1 ) == 0 ) 
	atm->act = ACCEPTOR;
      if ( strncmp ( atm->name, "OG", 2 ) == 0 
	   || strncmp ( atm->name, "OH", 2 ) == 0 
	   || strncmp ( atm->name, "AD", 2 ) == 0
	   || strncmp ( atm->name, "AE", 2 ) == 0 ) 
	atm->act = DONEPTOR;
    }
  /* The following code was added by PCS -- 2-Apr-01:
     I realize there are more effecient ways to do the following (as 
     was the case with the previous implementation), but the following
     is used to aid in human readibility and to make it easier to add
     new protein atom types. Also, there are cases of redundacy 
     (e.g. ASP OD1 and OD2) that are assigned the same radius, but are
     seperated for visual ease and possible future changes. */
  /* Exception to the above are hydrogens, which all have the same
     radius (RAD_H) and are parsed simply to assign their level */
  /* ADD ATOM: To add a new atom type, add the radius definition in
     inc/vdwrad.h, add the atom type assignment to the under the
     appropriate residue below, and add a name definition to
     inc/defs.h (at ADD ATOM) */

  /* Assign atom types and radii based on residue names */
#ifdef TRACE
  printf ("atom residue %d\n", atm->res );
#endif
  switch (atm->res) 
    {
    case ALA:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_ALAN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ALACA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ALAC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ALAO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_ALAOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_ALACB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case ACE:   /*added by Litian   03/17/2004 */
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ACEO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ACEC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"CH3") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ACECA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;     
    case ARG:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_ARGN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ARGCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ARGC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ARGO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_ARGOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_ARGCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_ARGCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_ARGCD;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"NE") == 0) {
	atm->level = EPSILON;
	atm->type = NE;
	atm->rad = RAD_ARGNE;
	atm->orbit = PL3;
	strcpy(atm->type_str,  "N.pl3");
      }
      if (strcmp(atm->name,"CZ") == 0) {
	atm->level = ZETA;
	atm->type = CZ;
	atm->rad = RAD_ARGCZ;
	atm->orbit = CAT;
	strcpy(atm->type_str,  "C.cat");
      }
      if (strcmp(atm->name,"NH1") == 0) {
	atm->level = ETA;
	atm->type = NH1;
	atm->rad = RAD_ARGNH1;
	atm->orbit = PL3;
	strcpy(atm->type_str,  "N.pl3");
      }
      if (strcmp(atm->name,"NH2") == 0) {
	atm->level = ETA;
	atm->type = NH2;
	atm->rad = RAD_ARGNH2;
	atm->orbit = PL3;
	strcpy(atm->type_str,  "N.pl3");
      }
      break;
    case ASN:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_ASNN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ASNCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ASNC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ASNO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_ASNOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_ASNCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_ASNCG;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"OD1") == 0) {
	atm->level = DELTA;
	atm->type = OD1;
	atm->rad = RAD_ASNOD1;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"ND2") == 0) {
	atm->level = DELTA;
	atm->type = ND2;
	atm->rad = RAD_ASNND2;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"AD1") == 0) {
	atm->level = DELTA;
	atm->type = AD1;
	atm->rad = RAD_ASNAD1;
	atm->orbit = AMBIG;
	strcpy(atm->type_str,  "Ambig");
      }
      if (strcmp(atm->name,"AD2") == 0) {
	atm->level = DELTA;
	atm->type = AD2;
	atm->rad = RAD_ASNAD2;
	atm->orbit = AMBIG;
	strcpy(atm->type_str,  "Ambig");
      }
      break;
    case ASP:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_ASPN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ASPCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ASPC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ASPO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_ASPOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_ASPCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_ASPCG;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"OD1") == 0) {
	atm->level = DELTA;
	atm->type = OD1;
	atm->rad = RAD_ASPOD1;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OD2") == 0) {
	atm->level = DELTA;
	atm->type = OD2;
	atm->rad = RAD_ASPOD2;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }     
      break;
    case CYS:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_CYSN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_CYSCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_CYSC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_CYSO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_CYSOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_CYSCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"SG") == 0) {
	atm->level = GAMMA;
	atm->type = SG;
	atm->rad = RAD_CYSSG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "S.3");
      }
      break;
    case GLN:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_GLNN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_GLNCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_GLNC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_GLNO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_GLNOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_GLNCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_GLNCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_GLNCD;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"OE1") == 0) {
	atm->level = EPSILON;
	atm->type = OE1;
	atm->rad = RAD_GLNOE1;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"NE2") == 0) {
	atm->level = EPSILON;
	atm->type = NE2;
	atm->rad = RAD_GLNNE2;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"AE1") == 0) {
	atm->level = EPSILON;
	atm->type = AE1;
	atm->rad = RAD_GLNAE2;
	atm->orbit = AMBIG;
	strcpy(atm->type_str,  "Ambig");
      }
      if (strcmp(atm->name,"AE2") == 0) {
	atm->level = EPSILON;
	atm->type = AE2;
	atm->rad = RAD_GLNAE2;
	atm->orbit = AMBIG;
	strcpy(atm->type_str,  "Ambig");
      }
      break;
    case GLU:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_GLUN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_GLUCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_GLUC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_GLUO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_GLUOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_GLUCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_GLUCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_GLUCD;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"OE1") == 0) {
	atm->level = EPSILON;
	atm->type = OE1;
	atm->rad = RAD_GLUOE1;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"OE2") == 0) {
	atm->level = EPSILON;
	atm->type = OE2;
	atm->rad = RAD_GLUOE2;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }      
      break;
    case GLY:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_GLYN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_GLYCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_GLYC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_GLYO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_GLYOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      break;
    case HIS:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_HISN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_HISCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_HISC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_HISO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_HISOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_HISCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_HISCG;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"ND1") == 0) {
	atm->level = DELTA;
	atm->type = ND1;
	atm->rad = RAD_HISND1;
	atm->orbit = HISN;
	strcpy(atm->type_str,  "N.pl3");
      }
      if (strcmp(atm->name,"CD2") == 0) {
	atm->level = DELTA;
	atm->type = CD2;
	atm->rad = RAD_HISCD2;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"CE1") == 0) {
	atm->level = EPSILON;
	atm->type = CE1;
	atm->rad = RAD_HISCE1;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"NE2") == 0) {
	atm->level = EPSILON;
	atm->type = NE2;
	atm->rad = RAD_HISNE2;
	atm->orbit = HISN;
	strcpy(atm->type_str,  "N.pl3");
      }
      break;
    case ILE:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_ILEN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_ILECA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_ILEC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_ILEO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_ILEOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_ILECB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG1") == 0) {
	atm->level = GAMMA;
	atm->type = CG1;
	atm->rad = RAD_ILECG1;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG2") == 0) {
	atm->level = GAMMA;
	atm->type = CG2;
	atm->rad = RAD_ILECG2;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strncmp(atm->name,"CD",2) == 0) {
	atm->level = DELTA;
	atm->type = CD1;
	atm->rad = RAD_ILECD1;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case LEU:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_LEUN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_LEUCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_LEUC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_LEUO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_LEUOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_LEUCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_LEUCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD1") == 0) {
	atm->level = DELTA;
	atm->type = CD1;
	atm->rad = RAD_LEUCD1;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD2") == 0) {
	atm->level = DELTA;
	atm->type = CD2;
	atm->rad = RAD_LEUCD2;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case LYS:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_LYSN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_LYSCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_LYSC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_LYSO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_LYSOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_LYSCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_LYSCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_LYSCD;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CE") == 0) {
	atm->level = EPSILON;
	atm->type = CE;
	atm->rad = RAD_LYSCE;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"NZ") == 0) {
	atm->level = ZETA;
	atm->type = NZ;
	atm->rad = RAD_LYSNZ;
	atm->orbit = SP4;
	strcpy(atm->type_str,  "N.4");
      }
      break;
    case MET:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_METN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_METCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_METC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_METO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_METOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_METCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_METCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"SD") == 0) {
	atm->level = DELTA;
	atm->type = SD;
	atm->rad = RAD_METSD;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "S.3");
      }
      if (strcmp(atm->name,"CE") == 0) {
	atm->level = EPSILON;
	atm->type = CE;
	atm->rad = RAD_METCE;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case PCA:  /* added by Litian He  03/17/2004  */
      /* most atoms are treated as those in PRO, only OE/OE1 is treated as OE1 in GLU */
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_PRON;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_PROCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_PROC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_PROO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strncmp(atm->name,"OE", 2) == 0) {
	atm->level = EPSILON;
	atm->type = OE1;
	atm->rad = RAD_GLNOE1;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_PROCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_PROCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_PROCD;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case PHE:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_PHEN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_PHECA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_PHEC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_PHEO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_PHEOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_PHECB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_PHECG;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CD1") == 0) {
	atm->level = DELTA;
	atm->type = CD1;
	atm->rad = RAD_PHECD1;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CD2") == 0) {
	atm->level = DELTA;
	atm->type = CD2;
	atm->rad = RAD_PHECD2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CE1") == 0) {
	atm->level = EPSILON;
	atm->type = CE1;
	atm->rad = RAD_PHECE1;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CE2") == 0) {
	atm->level = EPSILON;
	atm->type = CE2;
	atm->rad = RAD_PHECE2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CZ") == 0) {
	atm->level = ZETA;
	atm->type = CZ;
	atm->rad = RAD_PHECZ;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      break;
    case PRO:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_PRON;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_PROCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_PROC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_PROO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_PROOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_PROCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_PROCG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CD") == 0) {
	atm->level = DELTA;
	atm->type = CD;
	atm->rad = RAD_PROCD;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case SER:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_SERN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_SERCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_SERC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_SERO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_SEROXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_SERCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"OG") == 0) {
	atm->level = GAMMA;
	atm->type = OG;
	atm->rad = RAD_SEROG;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "O.3");
      }
      break;
    case THR:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_THRN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_THRCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_THRC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_THRO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_THROXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_THRCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"OG1") == 0) {
	atm->level = GAMMA;
	atm->type = OG1;
	atm->rad = RAD_THROG1;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "O.3");
      }
      if (strcmp(atm->name,"CG2") == 0) {
	atm->level = GAMMA;
	atm->type = CG2;
	atm->rad = RAD_THRCG2;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      break;
    case TRP:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_TRPN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_TRPCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_TRPC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_TRPO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_TRPOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_TRPCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_TRPCG;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }      
      if (strcmp(atm->name,"CD1") == 0) {
	atm->level = DELTA;
	atm->type = CD1;
	atm->rad = RAD_TRPCD1;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }      
      if (strcmp(atm->name,"CD2") == 0) {
	atm->level = DELTA;
	atm->type = CD2;
	atm->rad = RAD_TRPCD2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      if (strcmp(atm->name,"NE1") == 0) {
	atm->level = EPSILON;
	atm->type = NE1;
	atm->rad = RAD_TRPNE1;
	atm->orbit = PL3;
	strcpy(atm->type_str,  "N.pl3");
      }      
      if (strcmp(atm->name,"CE2") == 0) {
	atm->level = EPSILON;
	atm->type = CE2;
	atm->rad = RAD_TRPCE2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      if (strcmp(atm->name,"CE3") == 0) {
	atm->level = EPSILON;
	atm->type = CE3;
	atm->rad = RAD_TRPCE3;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      if (strcmp(atm->name,"CZ2") == 0) {
	atm->level = ZETA;
	atm->type = CZ2;
	atm->rad = RAD_TRPCZ2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      if (strcmp(atm->name,"CZ3") == 0) {
	atm->level = ZETA;
	atm->type = CZ3;
	atm->rad = RAD_TRPCZ3;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      if (strcmp(atm->name,"CH2") == 0) {
	atm->level = ETA;
	atm->type = CH2;
	atm->rad = RAD_TRPCH2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      }      
      break;
    case TYR:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_TYRN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_TYRCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_TYRC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_TYRO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_TYROXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_TYRCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG") == 0) {
	atm->level = GAMMA;
	atm->type = CG;
	atm->rad = RAD_TYRCG;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CD1") == 0) {
	atm->level = DELTA;
	atm->type = CD1;
	atm->rad = RAD_TYRCD1;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CD2") == 0) {
	atm->level = DELTA;
	atm->type = CD2;
	atm->rad = RAD_TYRCD2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CE1") == 0) {
	atm->level = EPSILON;
	atm->type = CE1;
	atm->rad = RAD_TYRCE1;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CE2") == 0) {
	atm->level = EPSILON;
	atm->type = CE2;
	atm->rad = RAD_TYRCE2;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"CZ") == 0) {
	atm->level = ZETA;
	atm->type = CZ;
	atm->rad = RAD_TYRCZ;
	atm->orbit = AR;
	strcpy(atm->type_str,  "C.ar");
      } 
      if (strcmp(atm->name,"OH") == 0) {
	atm->level = ETA;
	atm->type = OH;
	atm->rad = RAD_TYROH;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "O.3");
      } 
      break;
    case VAL:
      if (strcmp(atm->name,"N") == 0) {
	atm->level = ALPHA;
	atm->type = N;
	atm->rad = RAD_VALN;
	atm->orbit = AM;
	strcpy(atm->type_str,  "N.am");
      }
      if (strcmp(atm->name,"CA") == 0) {
	atm->level = ALPHA;
	atm->type = CA;
	atm->rad = RAD_VALCA;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"C") == 0) {
	atm->level = ALPHA;
	atm->type = C;
	atm->rad = RAD_VALC;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "C.2");
      }
      if (strcmp(atm->name,"O") == 0) {
	atm->level = ALPHA;
	atm->type = O;
	atm->rad = RAD_VALO;
	atm->orbit = SP2;
	strcpy(atm->type_str,  "O.2");
      }
      if (strcmp(atm->name,"OXT") == 0) {
	atm->level = ALPHA;
	atm->type = OXT;
	atm->rad = RAD_VALOXT;
	atm->orbit = CO2;
	strcpy(atm->type_str,  "O.co2");
      }
      if (strcmp(atm->name,"CB") == 0) {
	atm->level = BETA;
	atm->type = CB;
	atm->rad = RAD_VALCB;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }
      if (strcmp(atm->name,"CG1") == 0) {
	atm->level = GAMMA;
	atm->type = CG1;
	atm->rad = RAD_VALCG1;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }      
      if (strcmp(atm->name,"CG2") == 0) {
	atm->level = GAMMA;
	atm->type = CG2;
	atm->rad = RAD_VALCG2;
	atm->orbit = SP3;
	strcpy(atm->type_str,  "C.3");
      }      
      break;
    default:
      atm->type = UNKNOWN;
      /*printf ("atom name = %s [%7.3f,%7.3f,%7.3f]\n", atm->name,atm->pos[X],atm->pos[Y],atm->pos[Z]);*/
      fprintf(stderr, "Unknown atom type %s %s %s %s\n", atm->name, atm->residue, atm->residue_num, atm->atom_number);
      exit(EXIT_FAILURE);
      break;
    }
  
  if ((strncmp(atm->name,"H",1) == 0) || (strncmp(atm->name,"1",1) == 0)|| (strncmp(atm->name,"2",1) == 0)|| (strncmp(atm->name,"3",1) == 0)|| (strncmp(atm->name,"4",1) == 0)|| (strncmp(atm->name,"5",1) == 0)|| (strncmp(atm->name,"6",1) == 0)|| (strncmp(atm->name,"7",1) == 0)|| (strncmp(atm->name,"8",1) == 0)|| (strncmp(atm->name,"9",1) == 0)|| (strncmp(atm->name,"0",1) == 0)) {
    atm->rad = 1.0;
    atm->type = H;
    switch ( atm->name[1] )
      {
      case '\0':
      case 'A':
	atm->level = ALPHA;
	break;
      case 'B':
	atm->level = BETA;
	break;
      case 'G':
	atm->level = GAMMA;
	break;
      case 'D':
	atm->level = DELTA;
	break;
      case 'E':
	atm->level = EPSILON;
	break;
      case 'H':
	atm->level = ETA;
	break;
      case 'Z':
	atm->level = ZETA;
	break;
      default:
	break;
      }
  }
#ifdef TRACE
  printf ("atom name = %s [%7.3f,%7.3f,%7.3f] %f\n", atm->name,atm->pos[X],atm->pos[Y],atm->pos[Z], atm->rad);
#endif
  if ( atm->rad < 0 )
    {
      fprintf(stderr, "Unknown atom type %s %s %s %f\n", atm->name, atm->residue, atm->residue_num, atm->rad);
      exit(EXIT_FAILURE);
    }
}



int get_traj (FILE *traj_file, int number_of_protein_atoms, float *coords )
{
  char buf[256];
  int num_read = 0;
  int i;
  char* endptr;

  /* Assume box size is on line immediately following coordinates */
  /* NOTE: This gets the next line before checking the number of positions
     that were read.  That means we read the box size before exiting the loop*/
  while(fgets(buf, 256, traj_file) && num_read < 3*number_of_protein_atoms){
    /*printf("%s", buf); */
    for(i = 0; i < strlen(buf) - 1; i+= 8){
      endptr = buf + i;
      coords[num_read] = strtod(buf + i, &endptr);
      /*coords[num_read] = atof(buf + i);*/
      //      printf("num: %f\n", coords[num_read]);
      if(endptr != buf + i) ++num_read;
    }
  }
  return num_read / 3;
}

/* Assumes number of grid points = number of ints in index array */
void bump(float *grid, int num_grid_pts, float *prot_coords, 
	  int num_prot_coords, int *bumps, float *grid_min, float *grid_max, atom_pt atm, float tol)
{
  int i, j, k, z;
  int skip = 0;
  float tol_squared;
  float d_squared,
    tmp;
  float* grid_pt;
  float* atom_pos = prot_coords;

  for (i = 0; i < num_prot_coords; i++, atom_pos +=3)
    {
      skip = 0;
      for (k = 0; k < 3; ++k)
	{
	  if( atm[i].type == H || prot_coords[i*3+k] < grid_min[k] - (atm[i].rad + tol) || prot_coords[i*3+k] > grid_max[k] + (atm[i].rad + tol))
	    {
	      skip = 1;
	      break;
	    }
	}
      if (skip == 1) continue;
      grid_pt = grid;
      for (j = 0; j < num_grid_pts; j++, grid_pt +=3)
	{
	  if(bumps[j]) continue;
	  tol_squared = (atm[i].rad + tol) * (atm[i].rad + tol);
	  d_squared = 0;
	  for(z = 0; z < 3; ++z)
	    {
	      tmp = prot_coords[i*3+z] - grid[j*3+z];
	      d_squared += tmp*tmp;
	    }
	  if(d_squared < tol_squared)
	    {
	      ++bumps[j];
	    }
	}
    }
}




int main (int argc, char **argv)
{
  if ( argc < 12 )
    {
      printf ("%s <protein 1 ID> <protein pdb 1> <trajectory 1> <protein 2 ID> <protein pdb 2> <trajectory 2> <ligand pdb> <grid spacing> <shell radius> <probe radius> <minimum accessible volume main protein (%)> <maximum accessible volume secondary protein (%)>\n", argv[0] );
      return -1;
    }
  
  
  FILE  *protein_file_1, *protein_file_2, *trajectory_file_1, *trajectory_file_2, *ligand_file, *output_file_1, *output_file_2, *output_file_diff, *pymol_file, *grid_file_1, *grid_file_2, *grid_file_diff, *output_file_compare;
  char  protein_name_1[FILENAME_MAX],
    protein_name_2[FILENAME_MAX],
    trajectory_name_1[FILENAME_MAX],
    trajectory_name_2[FILENAME_MAX],
    ligand_name[FILENAME_MAX],
    output_file_name_1[FILENAME_MAX],
    output_file_name_2[FILENAME_MAX],
    output_file_name_diff[FILENAME_MAX],
    protein1_ID[FILENAME_MAX],
    protein2_ID[FILENAME_MAX],
    pymol_file_name[FILENAME_MAX],
    output_file_ID[FILENAME_MAX],
    grid_file_name_1[FILENAME_MAX],
    grid_file_name_2[FILENAME_MAX],
    grid_file_name_diff[FILENAME_MAX],
    output_file_name_compare[FILENAME_MAX];
  
  char line[256];
  char linebuf[256];
  float tol;
  float tol_squared, d_squared, tmp;
  tol_squared = tol*tol;

  float dist;
  int i, j, k;
  float x, y, z;
  int num_read;  
  int number_of_protein_atoms_1,
    number_of_protein_atoms_2,
    number_of_ligand_atoms,
    number_of_snapshots_1,
    number_of_snapshots_2,
    number_of_grid_pts;

  int max_number_of_ligand_atoms;
  int max_number_of_grid_points;
  float traj_occ_1, traj_occ_2, traj_diff, max_diff, min_diff, traj_range;

  int is_atom, is_hydro;

  int *bumps_1, *bumps_2, *bump_counts_1, *bump_counts_2, *bump_grid;
  int pruned_grid_counts;
  int water_counter = 0;
  int chain_counter = 0;
  int pdb_off = 0;
  
  float grid_spacing,
    shell_radius, shell_squared, min_acc_main, max_acc_alt;
  float *lig_coords = 0;
  float *grid_coords = 0;
  float *traj_coords_1 = 0;
  float *traj_coords_2 = 0;
  float *pruned_grid_coords = 0;  
  float *prot_coords = 0;

  float grid_x_min, grid_x_max, grid_y_min, grid_y_max, grid_z_min, grid_z_max, grid_x_range, grid_y_range, grid_z_range, grid_x_mid, grid_y_mid, grid_z_mid, grid_x_dim, grid_y_dim, grid_z_dim;
  float *grid_min = 0;
  float *grid_max = 0;
  
  float* grid_pt = grid_coords;
  float* lig_atom_pt = lig_coords;

  sprintf(protein1_ID, argv[1]);
  sprintf(protein_name_1, argv[2]);
  sprintf(trajectory_name_1, argv[3]);
  sprintf(protein2_ID, argv[4]);
  sprintf(protein_name_2, argv[5]);
  sprintf(trajectory_name_2, argv[6]);
  sprintf(ligand_name, argv[7]);
  

  atom_pt protein_atoms_1,
    protein_atoms_2;
  
  protein_atoms_1 =  (atom_pt) mymalloc ( 8000 * sizeof (atom_t) );
  protein_atoms_2 =  (atom_pt) mymalloc ( 8000 * sizeof (atom_t) );
  
  
  grid_x_min = grid_x_max = grid_y_min = grid_y_max = grid_z_min = grid_z_max = 0;
  grid_x_mid = grid_y_mid = grid_z_mid = 0;
  grid_x_dim = grid_y_dim = grid_z_dim = 0;

  grid_spacing = (float) atof (argv[8]);
  shell_radius = (float) atof (argv[9]);
  tol = (float) atof (argv[10]);
  min_acc_main = (float) atof (argv[11]);
  max_acc_alt = (float) atof (argv[12]);
  shell_squared = shell_radius*shell_radius;

  max_number_of_ligand_atoms = 8000;
  max_number_of_grid_points = 200000;

  number_of_protein_atoms_1 = 0;
  number_of_protein_atoms_2 = 0;
  number_of_ligand_atoms = 0;
  number_of_snapshots_1 = 0;
  number_of_snapshots_2 = 0;
  number_of_grid_pts = 0;

  bump_grid = 0;
  pruned_grid_counts = 0;
  bumps_1 = bumps_2 = 0;
  bump_counts_1 = bump_counts_2 = 0;

  lig_coords = (float*) malloc(3*max_number_of_ligand_atoms * sizeof(float));


  /* Open first protein file   */ 
  protein_file_1 = fopen (argv[2], "r");
  if (protein_file_1 == NULL)
    {
      printf ("unable to open file %s\n", argv[2]);
      return -1;
    }
  /* read all the lines in the pdb-file */
  while ( fgets ( linebuf, sizeof linebuf, protein_file_1 ) != NULL
	  && ! strkeq ( ".", linebuf, 1 ) 
	  && ! strkeq ( "END\n", linebuf, 4 ) 
	  && ! strkeq ( "end\n", linebuf, 4 ) 
	  && ! strkeq ( "END ", linebuf, 4 ) 
	  /* dont trigger on ENDx extensions */
	  && ! strkeq ( "end ", linebuf, 4 ) )
    {
      /* in this case also read the HETATMs, but no waters */
      {
	is_atom = strkeq ( "ATOM", linebuf, 4 )
	  || strkeq ( "atom", linebuf, 4 )
	  || ( ( strkeq ( "HETATM", linebuf, 6 )
		 || strkeq ( "hetatm", linebuf, 6 ) )
	       && strncmp ( "HOH", (char *) linebuf + 17, 3 ) != 0 );	  
      }
      
      is_hydro = ( toupper ( *column ( 14 ) ) == 68       /* 'D' or 'd' */
		   || toupper ( *column ( 14 ) ) == 72    /* 'H' or 'h' */
		   || toupper ( *column ( 14 ) ) == 81 ); /* 'Q' or 'q' */
      
      /* take care of only ATOM that no hydrogens and other junk is read */
      if ( is_atom 
	   && 1 == sscanf ( column ( 14 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name ) )
	{
	  protein_atoms_1[number_of_protein_atoms_1].pos[X] = (float) atof ( column ( 31 ) );
	  protein_atoms_1[number_of_protein_atoms_1].pos[Y] = (float) atof ( column ( 39 ) );
	  protein_atoms_1[number_of_protein_atoms_1].pos[Z] = (float) atof ( column ( 47 ) );
	  
	  /* get residue type, number, chainID, altloc, and insetion code */
	  strncpy ( protein_atoms_1[number_of_protein_atoms_1].residue, column ( 18 ), 3 );
	  protein_atoms_1[number_of_protein_atoms_1].residue[3] = '\0';
	  strncpy ( &protein_atoms_1[number_of_protein_atoms_1].alt_location, column ( 17 ), 1);
	  strncpy ( &protein_atoms_1[number_of_protein_atoms_1].chain_id, column ( 22 ), 1);
	  strncpy ( &protein_atoms_1[number_of_protein_atoms_1].insertion_code, column( 27 ), 1);
	  /*strncpy ( protein_atoms_1[number_of_protein_atoms_1].residue_num, column( 23 ), 4);*/
	  sscanf ( column ( 23 ), "%4s", protein_atoms_1[number_of_protein_atoms_1].residue_num ); /*toneroma 20FEB07 - changes made so that insertion codes work properly*/
	  
	  
	  if ( strkeq ( "ATOM", linebuf, 4 )
	       || strkeq ( "atom", linebuf, 4 ) )
	    {
	      /* assign_residue_type to current atom */
	      assign_residue_type ( &protein_atoms_1[number_of_protein_atoms_1] );
	      /* assign types and radii to current atom */
	      assign_type_and_radius ( &protein_atoms_1[number_of_protein_atoms_1] );
	      number_of_protein_atoms_1++;
	    }
	  else
	    {
	      /* this marks that this atom is a HETATM */
	      protein_atoms_1[number_of_protein_atoms_1].level = UNKNOWN;
	      protein_atoms_1[number_of_protein_atoms_1].res = HETATM;
	      protein_atoms_1[number_of_protein_atoms_1].act = NOTHING;
	      number_of_protein_atoms_1++;
      
	      switch ( (int) protein_atoms_1[number_of_protein_atoms_1].name[0] )
		{
		case 67:
		  protein_atoms_1[number_of_protein_atoms_1].type = C;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 80;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "C.het");
		  break;
		case 69:
		  protein_atoms_1[number_of_protein_atoms_1].type = FE;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 317;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Fe");
		  break;
		case 78:
		  protein_atoms_1[number_of_protein_atoms_1].type = N;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 350;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "N.het");
		  break;
		case 79:
		  protein_atoms_1[number_of_protein_atoms_1].type = O;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 530;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "O.het");
		  break;
		case 80:
		  protein_atoms_1[number_of_protein_atoms_1].type = P;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 317;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "P.het");
		  break;
		case 83:
		  protein_atoms_1[number_of_protein_atoms_1].type = S;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 80;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "S.het");
		  break;
		default:
		  protein_atoms_1[number_of_protein_atoms_1].type = UNKNOWN;
		  protein_atoms_1[number_of_protein_atoms_1].hydro = 317;
		  strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "HET");
		  break;
		}

	      /*****************************************************************
		handle the metal HETATM to count the metal hbond in scoring

                   metal_class  metal_name                    metal_hbond_distance
                    1            Ca, Na, K                       2.9 angstroms
                    2            Co, Cu, Fe, Mg, Mn, Ni, Zn      2.6 angstroms

                		       -- added by Litian He  12/02/2003  
	      ******************************************************************/

	      /* "CA" is a special case, both atom name and residue name should be "CA"  */
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'A' && 
		   *column ( 15 ) == ' ' && *column ( 18 ) == ' ' && 
		   *column ( 19 ) == 'C' && *column ( 20 ) == 'A' )
		 {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = CA_M;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_1;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Ca");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      /* we only need to check the atom name for all other metals */
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'O' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = CO;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Co");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'U' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = CU;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Cu");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'F' && *column ( 14 ) == 'E' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = FE;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 317;  /*  Sameer - is this deliberate or oversight ???*/
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Fe");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'K' && *column ( 14 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = K;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_1;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "K");
		      
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'M' && *column ( 14 ) == 'G' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = MG;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Mg");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'M' && *column ( 14 ) == 'N' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = MN;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Mn");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'N' && *column ( 14 ) == 'A' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = NA;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_1;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Na");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'N' && *column ( 14 ) == 'I' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = NI;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Ni");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'Z' && *column ( 14 ) == 'N' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_1[number_of_protein_atoms_1].name );
		      protein_atoms_1[number_of_protein_atoms_1].type = ZN;
		      protein_atoms_1[number_of_protein_atoms_1].act = METAL_2;
		      protein_atoms_1[number_of_protein_atoms_1].hydro = 635;
		      strcpy(protein_atoms_1[number_of_protein_atoms_1].type_str,  "Zn");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_1[number_of_protein_atoms_1].name, linebuf);
#endif
		  }

	      /******************* end of metal check ********************/

	      if ( protein_atoms_1[number_of_protein_atoms_1].name[0] == 'O' || protein_atoms_1[number_of_protein_atoms_1].name[0] == 'N' )
		{
		  if ( strncmp ( protein_atoms_1[number_of_protein_atoms_1].name + 1, "DD", 2 ) == 0 )
		    protein_atoms_1[number_of_protein_atoms_1].act = DONOR;
		  if ( strncmp ( protein_atoms_1[number_of_protein_atoms_1].name + 1, "1D", 2 ) == 0 )
		    protein_atoms_1[number_of_protein_atoms_1].act = DONOR;
		  if ( strncmp ( protein_atoms_1[number_of_protein_atoms_1].name + 1, "2D", 2 ) == 0 )
		    protein_atoms_1[number_of_protein_atoms_1].act = DONOR;
		  if ( strncmp ( protein_atoms_1[number_of_protein_atoms_1].name + 1, "AA", 2 ) == 0 )
		    protein_atoms_1[number_of_protein_atoms_1].act = ACCEPTOR;
		  if ( strncmp ( protein_atoms_1[number_of_protein_atoms_1].name + 1, "NN", 2 ) == 0 )
		    protein_atoms_1[number_of_protein_atoms_1].act = DONEPTOR;
		}
	      protein_atoms_1[number_of_protein_atoms_1].rad = radius ( protein_atoms_1[number_of_protein_atoms_1].type, UNKNOWN );
	    }
	}
    }
  fclose (protein_file_1);
  /*  printf("REMARK number of protein atoms: %d\n", number_of_protein_atoms_1);*/

  /* open second protein file */

  protein_file_2 = fopen (argv[5], "r");
  if (protein_file_2 == NULL)
    {
      printf ("unable to open file %s\n", argv[5]);
      return -1;
    }
  /* read all the lines in the pdb-file */
  while ( fgets ( linebuf, sizeof linebuf, protein_file_2 ) != NULL
	  && ! strkeq ( ".", linebuf, 1 ) 
	  && ! strkeq ( "END\n", linebuf, 4 ) 
	  && ! strkeq ( "end\n", linebuf, 4 ) 
	  && ! strkeq ( "END ", linebuf, 4 ) 
	  /* dont trigger on ENDx extensions */
	  && ! strkeq ( "end ", linebuf, 4 ) )
    {
      /* in this case also read the HETATMs, but no waters */
      {
	is_atom = strkeq ( "ATOM", linebuf, 4 )
	  || strkeq ( "atom", linebuf, 4 )
	  || ( ( strkeq ( "HETATM", linebuf, 6 )
		 || strkeq ( "hetatm", linebuf, 6 ) )
	       && strncmp ( "HOH", (char *) linebuf + 17, 3 ) != 0 );	  
      }
      
      is_hydro = ( toupper ( *column ( 14 ) ) == 68       /* 'D' or 'd' */
		   || toupper ( *column ( 14 ) ) == 72    /* 'H' or 'h' */
		   || toupper ( *column ( 14 ) ) == 81 ); /* 'Q' or 'q' */
      
      /* take care of only ATOM that no hydrogens and other junk is read */
      if ( is_atom 
	   && 1 == sscanf ( column ( 14 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name ) )
	{
	  protein_atoms_2[number_of_protein_atoms_2].pos[X] = (float) atof ( column ( 31 ) );
	  protein_atoms_2[number_of_protein_atoms_2].pos[Y] = (float) atof ( column ( 39 ) );
	  protein_atoms_2[number_of_protein_atoms_2].pos[Z] = (float) atof ( column ( 47 ) );
	  
	  /* get residue type, number, chainID, altloc, and insetion code */
	  strncpy ( protein_atoms_2[number_of_protein_atoms_2].residue, column ( 18 ), 3 );
	  protein_atoms_2[number_of_protein_atoms_2].residue[3] = '\0';
	  strncpy ( &protein_atoms_2[number_of_protein_atoms_2].alt_location, column ( 17 ), 1);
	  strncpy ( &protein_atoms_2[number_of_protein_atoms_2].chain_id, column ( 22 ), 1);
	  strncpy ( &protein_atoms_2[number_of_protein_atoms_2].insertion_code, column( 27 ), 1);
	  /*strncpy ( protein_atoms_2[number_of_protein_atoms_2].residue_num, column( 23 ), 4);*/
	  sscanf ( column ( 23 ), "%4s", protein_atoms_2[number_of_protein_atoms_2].residue_num ); /*toneroma 20FEB07 - changes made so that insertion codes work properly*/
	  
	  
	  if ( strkeq ( "ATOM", linebuf, 4 )
	       || strkeq ( "atom", linebuf, 4 ) )
	    {
	      /* assign_residue_type to current atom */
	      assign_residue_type ( &protein_atoms_2[number_of_protein_atoms_2] );
	      /* assign types and radii to current atom */
	      assign_type_and_radius ( &protein_atoms_2[number_of_protein_atoms_2] );
	      number_of_protein_atoms_2++;
	    }
	  else
	    {
	      /* this marks that this atom is a HETATM */
	      protein_atoms_2[number_of_protein_atoms_2].level = UNKNOWN;
	      protein_atoms_2[number_of_protein_atoms_2].res = HETATM;
	      protein_atoms_2[number_of_protein_atoms_2].act = NOTHING;
	      number_of_protein_atoms_2++;
      
	      switch ( (int) protein_atoms_2[number_of_protein_atoms_2].name[0] )
		{
		case 67:
		  protein_atoms_2[number_of_protein_atoms_2].type = C;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 80;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "C.het");
		  break;
		case 69:
		  protein_atoms_2[number_of_protein_atoms_2].type = FE;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 317;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Fe");
		  break;
		case 78:
		  protein_atoms_2[number_of_protein_atoms_2].type = N;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 350;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "N.het");
		  break;
		case 79:
		  protein_atoms_2[number_of_protein_atoms_2].type = O;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 530;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "O.het");
		  break;
		case 80:
		  protein_atoms_2[number_of_protein_atoms_2].type = P;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 317;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "P.het");
		  break;
		case 83:
		  protein_atoms_2[number_of_protein_atoms_2].type = S;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 80;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "S.het");
		  break;
		default:
		  protein_atoms_2[number_of_protein_atoms_2].type = UNKNOWN;
		  protein_atoms_2[number_of_protein_atoms_2].hydro = 317;
		  strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "HET");
		  break;
		}

	      /*****************************************************************
		handle the metal HETATM to count the metal hbond in scoring

                   metal_class  metal_name                    metal_hbond_distance
                    1            Ca, Na, K                       2.9 angstroms
                    2            Co, Cu, Fe, Mg, Mn, Ni, Zn      2.6 angstroms

                		       -- added by Litian He  12/02/2003  
	      ******************************************************************/

	      /* "CA" is a special case, both atom name and residue name should be "CA"  */
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'A' && 
		   *column ( 15 ) == ' ' && *column ( 18 ) == ' ' && 
		   *column ( 19 ) == 'C' && *column ( 20 ) == 'A' )
		 {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = CA_M;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_1;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Ca");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      /* we only need to check the atom name for all other metals */
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'O' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = CO;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Co");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'C' && *column ( 14 ) == 'U' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = CU;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Cu");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'F' && *column ( 14 ) == 'E' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = FE;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 317;  /*  Sameer - is this deliberate or oversight ???*/
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Fe");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'K' && *column ( 14 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = K;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_1;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "K");
		      
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'M' && *column ( 14 ) == 'G' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = MG;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Mg");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'M' && *column ( 14 ) == 'N' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = MN;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Mn");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'N' && *column ( 14 ) == 'A' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = NA;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_1;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Na");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_1 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'N' && *column ( 14 ) == 'I' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = NI;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Ni");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }
	      if ( *column ( 13 ) == 'Z' && *column ( 14 ) == 'N' && *column ( 15 ) == ' ' )
		  {
		      sscanf ( column ( 13 ), " %3s", protein_atoms_2[number_of_protein_atoms_2].name );
		      protein_atoms_2[number_of_protein_atoms_2].type = ZN;
		      protein_atoms_2[number_of_protein_atoms_2].act = METAL_2;
		      protein_atoms_2[number_of_protein_atoms_2].hydro = 635;
		      strcpy(protein_atoms_2[number_of_protein_atoms_2].type_str,  "Zn");
#ifdef TRACE_METAL
		      printf("\"%s\" - METAL_2 : %s", protein_atoms_2[number_of_protein_atoms_2].name, linebuf);
#endif
		  }

	      /******************* end of metal check ********************/

	      if ( protein_atoms_2[number_of_protein_atoms_2].name[0] == 'O' || protein_atoms_2[number_of_protein_atoms_2].name[0] == 'N' )
		{
		  if ( strncmp ( protein_atoms_2[number_of_protein_atoms_2].name + 1, "DD", 2 ) == 0 )
		    protein_atoms_2[number_of_protein_atoms_2].act = DONOR;
		  if ( strncmp ( protein_atoms_2[number_of_protein_atoms_2].name + 1, "1D", 2 ) == 0 )
		    protein_atoms_2[number_of_protein_atoms_2].act = DONOR;
		  if ( strncmp ( protein_atoms_2[number_of_protein_atoms_2].name + 1, "2D", 2 ) == 0 )
		    protein_atoms_2[number_of_protein_atoms_2].act = DONOR;
		  if ( strncmp ( protein_atoms_2[number_of_protein_atoms_2].name + 1, "AA", 2 ) == 0 )
		    protein_atoms_2[number_of_protein_atoms_2].act = ACCEPTOR;
		  if ( strncmp ( protein_atoms_2[number_of_protein_atoms_2].name + 1, "NN", 2 ) == 0 )
		    protein_atoms_2[number_of_protein_atoms_2].act = DONEPTOR;
		}
	      protein_atoms_2[number_of_protein_atoms_2].rad = radius ( protein_atoms_2[number_of_protein_atoms_2].type, UNKNOWN );
	    }
	}
    }
  fclose (protein_file_2);



  
  ligand_file = fopen (argv[7], "r");
  if (ligand_file == NULL)
    {
      printf ("unable to open file %s\n", argv[7]);
      return -1;
    }
  while ( fgets ( linebuf, sizeof linebuf, ligand_file ) != NULL
	  && ! strkeq ( ".", linebuf, 1 ) 
	  && ! strkeq ( "END\n", linebuf, 4 ) 
	  && ! strkeq ( "end\n", linebuf, 4 ) 
	  && ! strkeq ( "END ", linebuf, 4 ) 
	  /* dont trigger on ENDx extensions */
	  && ! strkeq ( "end ", linebuf, 4 ) )
    {
      if (strkeq ("ATOM", linebuf, 4)
	  || strkeq ("HETATM", linebuf, 6))
	{
	  if (number_of_ligand_atoms == 0)
	    {
	      grid_x_min = (float) atof ( column (31));
	      grid_x_max = (float) atof ( column (31));
	      grid_y_min = (float) atof ( column (39));
	      grid_y_max = (float) atof ( column (39));
	      grid_z_min = (float) atof ( column (47));
	      grid_z_max = (float) atof ( column (47));

	    }
	  

	  lig_coords[(number_of_ligand_atoms*3)] = (float) atof ( column (31) ) ;
	  if (lig_coords[(number_of_ligand_atoms*3)] < grid_x_min )
	    {
	      grid_x_min = lig_coords[(number_of_ligand_atoms*3)];
	    }
	  if (lig_coords[(number_of_ligand_atoms*3)] > grid_x_max )
	    {
	      grid_x_max = lig_coords[(number_of_ligand_atoms*3)];
	    }
	  lig_coords[(number_of_ligand_atoms*3)+1] = (float) atof ( column (39) ) ;
	  if (lig_coords[(number_of_ligand_atoms*3)+1] < grid_y_min )
	    {
	      grid_y_min = lig_coords[(number_of_ligand_atoms*3)+1];
	    }
	  if (lig_coords[(number_of_ligand_atoms*3)+1] > grid_y_max )
	    {
	      grid_y_max = lig_coords[(number_of_ligand_atoms*3)+1];
	    }
	  lig_coords[(number_of_ligand_atoms*3)+2] = (float) atof ( column (47) ) ;
	  if (lig_coords[(number_of_ligand_atoms*3)+2] < grid_z_min )
	    {
	      grid_z_min = lig_coords[(number_of_ligand_atoms*3)+2];
	    }
	  if (lig_coords[(number_of_ligand_atoms*3)+2] > grid_z_max )
	    {
	      grid_z_max = lig_coords[(number_of_ligand_atoms*3)+2];
	    }

	  /*	  printf("# %d %7.3f %7.3f %7.3f\n", number_of_ligand_atoms+1, lig_coords[(number_of_ligand_atoms*3)], lig_coords[(number_of_ligand_atoms*3)+1], lig_coords[(number_of_ligand_atoms*3)+2]);*/
	  number_of_ligand_atoms++;
	}

    }
  fclose (ligand_file);
  grid_x_min-=shell_radius;
  grid_x_max+=shell_radius;
  grid_y_min-=shell_radius;
  grid_y_max+=shell_radius;
  grid_z_min-=shell_radius;
  grid_z_max+=shell_radius;
  


  grid_x_mid = ((grid_x_min) + (grid_x_max))/2;
  grid_y_mid = ((grid_y_min) + (grid_y_max))/2;
  grid_z_mid = ((grid_z_min) + (grid_z_max))/2;

  /*  grid_x_dim = (abs(grid_x_min) + abs (grid_x_max))/2;
  grid_y_dim = (abs(grid_y_min) + abs (grid_y_max))/2;
  grid_z_dim = (abs(grid_z_min) + abs (grid_z_max))/2;*/

  grid_x_dim = ((grid_x_min) + abs (grid_x_max))/2;
  grid_y_dim = ((grid_y_min) + abs (grid_y_max))/2;
  grid_z_dim = ((grid_z_min) + abs (grid_z_max))/2;


  /*  printf("REMARK number of ligand atoms: %d\n", number_of_ligand_atoms);
      printf("REMARK box_x %7.3f %7.3f %7.3f\nREMARK box_y %7.3f %7.3f %7.3f\nREMARK box_z %7.3f %7.3f %7.3f\n", grid_x_min, grid_x_max, grid_x_mid, grid_y_min, grid_y_max, grid_y_mid, grid_z_min, grid_z_max, grid_z_mid);*/
  
  /* Create a grid of the spacing specified, within the radius specified of any ligand atoms */
  grid_x_range = (1+(grid_x_max-grid_x_min)/grid_spacing);
  grid_y_range = (1+(grid_y_max-grid_y_min)/grid_spacing);
  grid_z_range = (1+(grid_z_max-grid_z_min)/grid_spacing);
  
  max_number_of_grid_points = grid_x_range * grid_y_range * grid_z_range;
  /*
  printf("Title:\n");
  printf("Mid:\t%5.3f\t%5.3f\t%5.3f\n", grid_x_mid, grid_y_mid, grid_z_mid);
  printf("Dim:\t%d\t%d\t%d\n", (int)grid_x_range, (int)grid_y_range, (int)grid_z_range);
  printf("Spacing:\t%5.3f\n", grid_spacing);
  */

  /*  printf("REMARK max num of grid points = %d\n", max_number_of_grid_points)
      printf("REMARK box_x %7.3f\nREMARK box_y %7.3f\nREMARK box_z %7.3f\n", grid_x_range/2, grid_y_range/2, grid_z_range/2);*/
			       
  grid_coords = (float*) malloc(3*max_number_of_grid_points * sizeof(float));

  grid_min = (float*) malloc(3 * sizeof(float));
  grid_max = (float*) malloc(3 * sizeof(float));

  grid_min[0] = grid_x_min;
  grid_min[1] = grid_y_min;
  grid_min[2] = grid_z_min;

  grid_max[0] = grid_x_max;
  grid_max[1] = grid_y_max;
  grid_max[2] = grid_z_max;


  for (z = grid_z_min; z <= grid_z_max; z+=grid_spacing)
    for (y = grid_y_min; y <= grid_y_max; y+=grid_spacing)
      for (x = grid_x_min; x <= grid_x_max; x+=grid_spacing)
	{
	  grid_coords[number_of_grid_pts*3] = x;
	  grid_coords[number_of_grid_pts*3+1] = y;
	  grid_coords[number_of_grid_pts*3+2] = z;
	  /* printf("%d %7.3f %7.3f %7.3f\n", number_of_grid_pts, grid_coords[number_of_grid_pts*3], grid_coords[number_of_grid_pts*3+1], grid_coords[number_of_grid_pts*3+2]);*/
	  number_of_grid_pts++;
	}
  /*  printf("REMARK initial number of grid points: %d\n", number_of_grid_pts);*/
  /* prune cavity grid to within radius of ligand atoms */
  /*  for (i = 0; i < number_of_grid_pts; ++i, grid_pt+=3)
      for (j = 0; j < number_of_ligand_atoms; ++j, lig_atom_pt+=3)
      {
      d_squared = 0;
      for(k = 0; k < 3; ++k)
      {
      tmp = grid_pt[k] - lig_atom_pt[k];
      d_squared += tmp*tmp;
      }
      dist = (float) sqrt(d_squared);
      printf("dist: %f\n", dist);
      if(d_squared < tol_squared) ++keep_grid[j];
      } 
  */  

    if (number_of_grid_pts >= 259900)
    {
      printf ("WARNING: Grid too large for PDB view. Reduce grid size or increase spacing\n");
      /*  pdb_off = 1; uncomment this out so that extra large useless pdb files won't be created*/
      /*      return 0; will currently allow this to go through since ascii grid will be implemented soon */
    }

  if (number_of_grid_pts >= 100000)
    {
      printf ("WARNING: Grid very large for PDB view. May not load properly or quickly in Pymol. Reduce grid size or increase spacing\n");
    }

  
  bump_grid = (int*) malloc(number_of_grid_pts * sizeof(int));
  pruned_grid_coords = (float*) malloc(3*max_number_of_grid_points * sizeof(float));

  for(i = 0; i < number_of_grid_pts; ++i, grid_pt += 3){
    lig_atom_pt = lig_coords;
    for(j = 0; j < number_of_ligand_atoms; ++j, lig_atom_pt += 3){
      if(bump_grid[i]) continue;
      d_squared = 0;
      for(k = 0; k < 3; ++k){
        tmp = grid_coords[i*3+k] - lig_coords[j*3+k];
        d_squared += tmp*tmp;
      }
      //printf("dist: %5.3f\n", sqrt(d_squared));

      if(d_squared < shell_squared)
	{
	  for(k = 0; k < 3; ++k)
	    {
	      pruned_grid_coords[pruned_grid_counts*3+k] = grid_coords[i*3+k];
	      /*	      printf("%f %f\n", grid_coords[i*3+k], pruned_grid_coords[pruned_grid_counts*3+k]);*/
	    }
	  pruned_grid_counts++;
	  ++bump_grid[i];
	  continue;
	}
    } 
  }
  
  /*  printf("REMARK pruned grid points: %d\n", pruned_grid_counts);
      printf("REMARK probe radius: %f\n", tol);*/
  
  /*  for (i = 0; i < pruned_grid_counts; i++)
      {
      printf ("%d\t%7.3f\t%7.3f\t%7.3f\n", i, pruned_grid_coords[i*3], pruned_grid_coords[i*3+1], pruned_grid_coords[i*3+2]);
      }
  */
  
  /* read trajectory 1 */
  num_read = 0;
  
  trajectory_file_1 = fopen (argv[3], "r");
  if (trajectory_file_1 == NULL)
    {
      printf ("unable to open file %s\n", argv[3]);
      return -1;
    }
  
  traj_coords_1 = (float*) malloc(3*number_of_protein_atoms_1 * sizeof(float));
  bumps_1 = (int*) malloc(number_of_grid_pts * sizeof(int));
  bump_counts_1 = (int*) malloc(number_of_grid_pts * sizeof(int));
  memset(bump_counts_1, 0, number_of_grid_pts * sizeof(int));
  
  while(!feof(trajectory_file_1)){
    num_read = get_traj(trajectory_file_1, number_of_protein_atoms_1, traj_coords_1);
    if(num_read == 0) break;
    
    if(num_read > 0 && num_read < number_of_protein_atoms_1){
      fprintf(stderr, "REMARK Number of positions read in trajectory: %d\n", num_read);
      return -1;
    }
    
    /* find which grid points bump for each snapshot */
    
    memset(bumps_1, 0, number_of_grid_pts * sizeof(int));
    
    bump(grid_coords, number_of_grid_pts, traj_coords_1, number_of_protein_atoms_1, bumps_1, grid_min, grid_max, protein_atoms_1, 
tol);
    
    for(i = 0; i < number_of_grid_pts; ++i) if(bumps_1[i]){
      ++bump_counts_1[i];
    }    
    /*
      void bump(float *grid, int num_grid_pts, float *prot_coords, 
      int num_prot_coords, int *bumps, float *grid_min, float *grid_max) */
    
    
    /*    printf("Snapshots analyzed: %d \n", number_of_snapshots);*/
    fprintf(stderr,"Snapshots of trajectory 1 analyzed: %d \n", number_of_snapshots_1);
    ++number_of_snapshots_1;
  }
  /*  printf("REMARK Number of snapshots %d\n", number_of_snapshots);*/
  fclose (trajectory_file_1);
  
  
  
  /* read trajectory 2 */
  num_read = 0;
  
  trajectory_file_2 = fopen (argv[6], "r");
  if (trajectory_file_2 == NULL)
    {
      printf ("unable to open file %s\n", argv[6]);
      return -1;
    }
  
  traj_coords_2 = (float*) malloc(3*number_of_protein_atoms_2 * sizeof(float));
  bumps_2 = (int*) malloc(number_of_grid_pts * sizeof(int));
  bump_counts_2 = (int*) malloc(number_of_grid_pts * sizeof(int));
  memset(bump_counts_2, 0, number_of_grid_pts * sizeof(int));
  
  while(!feof(trajectory_file_2)){
    num_read = get_traj(trajectory_file_2, number_of_protein_atoms_2, traj_coords_2);
    if(num_read == 0) break;
    
    if(num_read > 0 && num_read < number_of_protein_atoms_2){
      fprintf(stderr, "REMARK Number of positions read in trajectory: %d\n", num_read);
      return -1;
    }
    
    /* find which grid points bump for each snapshot */
    
    memset(bumps_2, 0, number_of_grid_pts * sizeof(int));
    
    bump(grid_coords, number_of_grid_pts, traj_coords_2, number_of_protein_atoms_2, bumps_2, grid_min, grid_max, protein_atoms_2, 
tol);
    
    for(i = 0; i < number_of_grid_pts; ++i) if(bumps_2[i]){
      ++bump_counts_2[i];
    }    
    /*
      void bump(float *grid, int num_grid_pts, float *prot_coords, 
      int num_prot_coords, int *bumps, float *grid_min, float *grid_max) */

    
    /*    printf("Snapshots analyzed: %d \n", number_of_snapshots);*/
    fprintf(stderr,"Snapshots of trajectory 2 analyzed: %d \n", number_of_snapshots_2);
    ++number_of_snapshots_2;
  }
  /*  printf("REMARK Number of snapshots %d\n", number_of_snapshots);*/
  fclose (trajectory_file_2);
  
  /* section deals with grid view */
  /*  printf("Values:\n");*/
  /* end grid view */  

  /*  for ( i = 0; i < grid_z_range; i++)
      for ( j = 0; j < grid_y_range; j++)
      for (k = 0; k < grid_z_range; k++)
	{
	  
	}*/

  sprintf(output_file_name_diff, "%s_%s_%.2f_%.2f_%.2f_diff.pdb", protein1_ID, protein2_ID, grid_spacing, shell_radius, tol);

  sprintf(output_file_name_compare, "%s_%s_%.2f_%.2f_%.2f_compare.pdb", protein1_ID, protein2_ID, grid_spacing, shell_radius, tol);

  sprintf(grid_file_name_diff, "%s_%s_%.2f_%.2f_%.2f.agd", protein1_ID, protein2_ID, grid_spacing, shell_radius, tol);

  sprintf(output_file_ID, "%s_%s_%.2f_%.2f_%.2f", protein1_ID, protein2_ID, grid_spacing, shell_radius, tol);

  sprintf(output_file_name_1, "%s_%.2f_%.2f_%.2f.pdb", protein1_ID, grid_spacing, shell_radius, tol);

  sprintf(grid_file_name_1, "%s_%.2f_%.2f_%.2f.agd", protein1_ID, grid_spacing, shell_radius, tol);

  sprintf(output_file_name_2, "%s_%.2f_%.2f_%.2f.pdb", protein2_ID, grid_spacing, shell_radius, tol);

  sprintf(grid_file_name_2, "%s_%.2f_%.2f_%.2f.agd", protein2_ID, grid_spacing, shell_radius, tol);
  if (pdb_off == 0)
    {
      output_file_1 = fopen ( output_file_name_1, "w" );
      if ( output_file_1 == NULL ) {
	printf("Could not open the file: %s\n", output_file_name_1);
      }
      
      output_file_2 = fopen ( output_file_name_2, "w" );
      if ( output_file_2 == NULL ) {
	printf("Could not open the file: %s\n", output_file_name_2);
      }
      
      output_file_diff = fopen ( output_file_name_diff, "w" );
      if ( output_file_diff == NULL ) {
	printf("Could not open the file: %s\n", output_file_name_diff);
      }
      
      output_file_compare = fopen ( output_file_name_compare, "w" );
      if ( output_file_compare == NULL ) {
	printf("Could not open the file: %s\n", output_file_name_compare);
      }
    }
  
  grid_file_1 = fopen ( grid_file_name_1, "w" );
  if ( grid_file_1 == NULL ) {
    printf("Could not open the file: %s\n", grid_file_name_1);
  }

  grid_file_2 = fopen ( grid_file_name_2, "w" );
  if ( grid_file_2 == NULL ) {
    printf("Could not open the file: %s\n", grid_file_name_2);
  }

  grid_file_diff = fopen ( grid_file_name_diff, "w" );
  if ( grid_file_diff == NULL ) {
    printf("Could not open the file: %s\n", grid_file_name_diff);
  }
  
  
  fprintf(output_file_1,"REMARK Grid: %.2f; Shell: %.2f; Probe: %.2f\n", grid_spacing, shell_radius, tol);
  fprintf(output_file_1,"REMARK Min primary available: %.2f; Max secondary available: %.2f\n", min_acc_main, max_acc_alt );

  fprintf(output_file_2,"REMARK Grid: %.2f; Shell: %.2f; Probe: %.2f\n", grid_spacing, shell_radius, tol);
  fprintf(output_file_2,"REMARK Min primary available: %.2f; Max secondary available: %.2f\n", min_acc_main, max_acc_alt );

  fprintf(output_file_diff,"REMARK Grid: %.2f; Shell: %.2f; Probe: %.2f\n", grid_spacing, shell_radius, tol);
  fprintf(output_file_diff,"REMARK Min primary available: %.2f; Max secondary available: %.2f\n", min_acc_main, max_acc_alt );

  fprintf(output_file_compare,"REMARK Grid: %.2f; Shell: %.2f; Probe: %.2f\n", grid_spacing, shell_radius, tol);
  fprintf(output_file_compare,"REMARK Min primary available: %.2f; Max secondary available: %.2f\n", min_acc_main, max_acc_alt );



  fprintf(grid_file_1, "Title:\n");
  fprintf(grid_file_1, "Mid:\t%5.3f\t%5.3f\t%5.3f\n", grid_x_mid, grid_y_mid, grid_z_mid);
  fprintf(grid_file_1, "Dim:\t%d\t%d\t%d\n", (int)grid_x_range, (int)grid_y_range, (int)grid_z_range);
  fprintf(grid_file_1,"Spacing:\t%5.3f\n", grid_spacing);
  fprintf(grid_file_1, "Values:\n");

  fprintf(grid_file_2, "Title:\n");
  fprintf(grid_file_2, "Mid:\t%5.3f\t%5.3f\t%5.3f\n", grid_x_mid, grid_y_mid, grid_z_mid);
  fprintf(grid_file_2, "Dim:\t%d\t%d\t%d\n", (int)grid_x_range, (int)grid_y_range, (int)grid_z_range);
  fprintf(grid_file_2,"Spacing:\t%5.3f\n", grid_spacing);
  fprintf(grid_file_2, "Values:\n");

  fprintf(grid_file_diff, "Title:\n");
  fprintf(grid_file_diff, "Mid:\t%5.3f\t%5.3f\t%5.3f\n", grid_x_mid, grid_y_mid, grid_z_mid);
  fprintf(grid_file_diff, "Dim:\t%d\t%d\t%d\n", (int)grid_x_range, (int)grid_y_range, (int)grid_z_range);
  fprintf(grid_file_diff,"Spacing:\t%5.3f\n", grid_spacing);
  fprintf(grid_file_diff, "Values:\n");
  
  
  max_diff = -10.0;
  min_diff = 10.0;
  traj_occ_1 = 0.0;
  traj_occ_2 = 0.0;
  traj_diff = 0.0;
  traj_range = 0.0;

  
  for (i = 0; i < number_of_grid_pts; i++)
    {
      /*          printf("%d\t%7.3f\t%7.3f\t%7.3f\t%d\t%.3f\n", i+1, grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], bump_counts[i],(((float) bump_counts[i])/(float)number_of_snapshots));*/
      /*      printf("HETATM%5d  O   HOH  %4d    %8.3f%8.3f%8.3f  1.00 %5.3f\n", i+1, i+1, grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], (((float)number_of_snapshots - (float) bump_counts[i])/(float)number_of_snapshots));*/

      if (water_counter == 9999)
	{
	  chain_counter++;
	  water_counter = 0;
	}
      /*      printf("%5.3f\n", (float) bumps[i]);*/
      traj_occ_1 =  (((float) bump_counts_1[i])/(float)number_of_snapshots_1);

      traj_occ_2 =  (((float) bump_counts_2[i])/(float)number_of_snapshots_2);

      traj_diff =  (((float) bump_counts_1[i])/(float)number_of_snapshots_1) - (((float) bump_counts_2[i])/(float)number_of_snapshots_2);
      if (traj_diff > max_diff)
	{
	  max_diff = traj_diff;
	}
      if (traj_diff < min_diff)
	{
	  min_diff = traj_diff;
	}
      
      fprintf(grid_file_1,"%5.3f\n", traj_occ_1);
      fprintf(grid_file_2,"%5.3f\n", traj_occ_2);
      fprintf(grid_file_diff,"%5.3f\n", traj_diff);
      
      if(pdb_off == 0)
	{
	  fprintf(output_file_1, "HETATM%5d  O   HOH %1c%4d    %8.3f%8.3f%8.3f  1.00% 5.3f\n", water_counter+1, 65+chain_counter, water_counter+1,  grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], traj_occ_1);
	  fprintf(output_file_2, "HETATM%5d  O   HOH %1c%4d    %8.3f%8.3f%8.3f  1.00% 5.3f\n", water_counter+1, 65+chain_counter, water_counter+1,  grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], traj_occ_2);
	  
	  if(traj_diff != 0.0)
	    {
	      fprintf(output_file_diff, "HETATM%5d  O   HOH %1c%4d    %8.3f%8.3f%8.3f  1.00% 5.3f\n", water_counter+1, 65+chain_counter, water_counter+1,  grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], traj_diff);
	    }
	  
	  if (((1.0 - traj_occ_1) >= min_acc_main) && ((1.0 - traj_occ_2) <= max_acc_alt))
	    {
	      fprintf(output_file_compare, "HETATM%5d  O   HOH %1c%4d    %8.3f%8.3f%8.3f  1.00% 5.3f\n", water_counter+1, 65+chain_counter, water_counter+1,  grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], 1.0 - traj_occ_1);
	    }
	  
	  if (((1.0 - traj_occ_2) >= min_acc_main) && ((1.0-traj_occ_1) <= max_acc_alt))
	    {
	      fprintf(output_file_compare, "HETATM%5d  O   HOH %1c%4d    %8.3f%8.3f%8.3f  1.00% 5.3f\n", water_counter+1, 65+chain_counter, water_counter+1,  grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], -1.0 + traj_occ_2);
	    }
	}
      water_counter++;
    }

  if(pdb_off==0)
    {
      fclose(output_file_1);
      fclose(output_file_2);
      fclose(output_file_diff);
      fclose(output_file_compare);
    }
  fclose(grid_file_1);
  fclose(grid_file_2);
  fclose(grid_file_diff);

  traj_range = max_diff - min_diff;
  
  

  sprintf(pymol_file_name, "pymol_%s.pml", output_file_ID);
  
  pymol_file = fopen ( pymol_file_name, "w" );
  if ( pymol_file == NULL ) {
    printf("Could not open the file: %s\n", pymol_file_name);
  }
  

  fprintf(pymol_file, "load %s, %s\n", protein_name_1, protein1_ID);
  fprintf(pymol_file, "load %s, %s\n", protein_name_2, protein2_ID);
  fprintf(pymol_file, "load %s, difference\n", output_file_name_diff);
  fprintf(pymol_file, "load %s, compare\n", output_file_name_compare);
  fprintf(pymol_file, "load %s, ligand\n", ligand_name);
  fprintf(pymol_file, "util.cba(154,\"%s\",_self=cmd)\n", protein1_ID);
  fprintf(pymol_file, "cmd.hide(\"lines\"     ,\"all\")\n");
  fprintf(pymol_file, "cmd.show(\"cartoon\"   ,\"%s\")\n", protein1_ID);
  fprintf(pymol_file, "cmd.show(\"cartoon\"   ,\"%s\")\n", protein2_ID);
  fprintf(pymol_file, "cmd.show(\"sticks\"    ,\"ligand\")\n");
  fprintf(pymol_file, "cmd.hide(\"nonbonded\" ,\"difference\")\n");
  fprintf(pymol_file, "cmd.show(\"nb_spheres\",\"difference\")\n");
  fprintf(pymol_file, "cmd.hide(\"nonbonded\" ,\"compare\")\n");
  fprintf(pymol_file, "cmd.show(\"spheres\",\"compare\")\n");
  fprintf(pymol_file, "color deepblue, difference and b>0\n");
  fprintf(pymol_file, "color marine, difference and b>%.3f\n", max_diff*1.0/3.0);
  fprintf(pymol_file, "color cyan, difference and b>%.3f\n", max_diff*2.0/3.0);
  fprintf(pymol_file, "color red, difference and b<0\n");
  fprintf(pymol_file, "color orange, difference and b<%.3f\n", min_diff*1.0/3.0);
  fprintf(pymol_file, "color yellow, difference and b<%.3f\n", min_diff*2.0/3.0);
  fprintf(pymol_file, "color blue, compare and b<0\n");
  fprintf(pymol_file, "log_close\n");

  fclose(pymol_file);

  /* grid view
    {
  */
      /*          printf("%d\t%7.3f\t%7.3f\t%7.3f\t%d\t%.3f\n", i+1, grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], bump_counts[i],(((float) bump_counts[i])/(float)number_of_snapshots));*/
      /*      printf("HETATM%5d  O   HOH  %4d    %8.3f%8.3f%8.3f  1.00 %5.3f\n", i+1, i+1, grid_coords[i*3], grid_coords[i*3+1], grid_coords[i*3+2], (((float)number_of_snapshots - (float) bump_counts[i])/(float)number_of_snapshots));*/
      /* grid view
      printf("%5.3f\n", (((float) bump_counts[i])/(float)number_of_snapshots));
      end grid view */
      /*
	}
	end grid view */
  /*  fclose (trajectory_file_1);*/

  if(traj_coords_1) free(traj_coords_1);  
  traj_coords_1 = 0;
  if(traj_coords_2) free(traj_coords_2);  
  traj_coords_2 = 0;

}

  
