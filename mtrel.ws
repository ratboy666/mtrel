..---!----!----!----!----!----!----!----!----!----!----!--------R
MTREL.WS

     Thå  HITECÈ  Ã compileò 3.0¹ appearó tï bå onå oæ  thå  besô 
choiceó  foò locaì Ã developmenô undeò CP/M-8° (witè á  Z80)®  Iô 
appearó tï generatå "reasonable¢ Z8° code¬  haó á reasonablå run-
timå library¬ anä actuallù supportó ANSÉ prototypes.

     However¬  likå  manù similaò productó oæ thå day¬  iô styleó 
itselæ aó á completå "system"®  Iô haó á Ã  compiler¬  Assembler¬ 
anä Linker® Thå ideá ió thaô useò buyó intï thå wholå system.

     Sincå HITECÈ Ã (likå Whitesmithó C© runó iî separatå passes¬ 
anä alwayó produceó assembler¬ thå assembleò caî bå mungeä intï á 
formaô  suitablå foò anotheò system®  É choså Microsofô  M80/L80® 
REÌ  fileó arå alsï useä bù Digitaì Researcè tooló likå RMAC¬  sï 
thió giveó uó morå flexibility.

     The HITECH C driver (C.COM) can be run like:

     Ã  -Ó HELLO.Ã (oò Ã -Ó -Ï HELLO.Ã iæ thå peepholå  optimizeò 
                    is desired)

     Thió produceó á HELLO.AÓ file¬ whicè caî bå processeä bù thå 
HITECÈ ZAÓ assembler® É generateä á utility:

     AS2MAC HELLO.AS >HELLO.MAC

     AS2MAÃ  convertó  aî AÓ filå produceä bù Ã intï  MAÃ  formaô 
suitablå  foò  Microsofô  M80®  NOTE:‚ Thå '>§  characteò  ió  noô 
optional®  AS2MAÃ  alwayó  writeó thå outpuô tï thå  consolå  buô 
redirectioî  ió  supported®  AS2MAC.Ã  ió included®  Therå  ió  á 
versioî  insidå  LIBCREL.LBÒ  -- ignorå thaô onå (iô  ió  ouô  oæ 
date).

     Thå AÓ fileó produceä bù HITECÈ Ã havå forwarä referencå (jğ 
1f..®  1:© labeló whicè M8° doesn'ô support®  Alsï psect¬  .high® 
.low®  anä  &¬  Iæ  thå  -Ï (optimize© optioî  ió  used¬  symboló 
beginninç  witè  Ì arå generateä anä conflicô witè  lower-caså  ì 
symboló  (M8°  ió  noô caså  sensitive)®  Floatinç  poinô  (deff© 
constantó  arå  replaceä witè fouò  bytå  sequences®   globaì  ió 
converteä  tï  exterî  iæ needed®  Extendeä conditioî  codeó  arå 
replaceä (eç jğ llt)®  Tï accomplisè thå conversion¬ AS2MAÃ takeó 
twï  passeó oveò thå AÓ file®  Iô alwayó writeó thå finaì MAÃ  tï 
stdout®  AS2MAÃ ió NOÔ á generaì utilitù -- iô ONLÙ convertó  thå 
outpuô froí C® Afteò thå conversion:

     M80 =HELLO

and link with:

     L80 ZCRTCPM,HELLO,HELLO/N/E

É  converteä thå Ã librarù tï LIBC.REL¬  anä thå floatinç librarù 
tï LIBF.REL®  Notå thaô ZCRTCPÍ haó á .REQUIRÅ LIBC¬  Iæ floatinç 
point is used, LIBF/S must be added to the link.
Š     Sincå   thå  L8°  linkeò  onlù  understandó  ¶   significanô 
characters¬ thå followinç nameó arå changeä (anä STRING.È fixed):

     strncpy -> stncpy
     strncat -> stncat
     strncmp -> stncmp

     Somå  internaì  nameó  arå changeä aó  welì  -- thå  updateä 
librarù sourcå ió provided.

     UEXIT.MAÃ  haó  beeî  modifieä tï writå thå returî  codå  tï 
locatioî 0x7e¬  noô 0x8° aó thå originaì HiTecè codå did® 0x8° ió 
destroyeä bù thå CCĞ beforå thå nexô prograí ió executed® 0x7å ió 
safer.

     É  alsï patcheä thå driveò (C.COM© tï refeò tï LINQ.COÍ  anä 
CREQ.COÍ tï avoiä problemó wheî combininç witè otheò tool-chains® 
LINK.COM is then named LINQ.COM, and CREF.COM is named CREQ.COM.

     Azteã  Ã  ió thå nexô mosô usefuì Ã compileò foò  mù  needs® 
But¬  Azteã doeó noô offeò functioî prototypes® Whitesmith'ó Ã ió 
whaô É useä "iî thå day"¬ buô iô toï ió non-ANSÉ (nï prototypes)¬ 
haó  á  non-standarä  library¬  á  verù  lock-iî  toolchaiî  (thå 
assembleò  ió  somethinç calleä  "A-natural")®  Whitesmithó  alsï 
takeó  great‚ (á 62KÂ TPÁ ió minimal© deaì oæ memory¬  anä ió 808° 
only®  Hi-Tecè  operateó witè CP/NEÔ 1.2¬  whicè ió mù  preferreä 
environment®  Azteã generateó botè 808° anä Z8° --  Hi-Tecè Ã  ió 
Z8° only® Howeveò É caî accepô thaô limitation.

     Foò  thoså reasons¬  É selecteä Hi-Tecè aó mù Ã compileò  oæ 
choice®  However¬  É  dï noô wanô tï leavå MACRO-80¬  aó thaô  ió 
still‚ preferreä assembleò systeí oî CP/M®  Thuó thå MTREÌ projecô 
waó born.

     Tï updatå á modulå iî á librarù witè lib8° uså thå followinç 
approach®  Foò example¬  leô uó saù yoõ wanô tï updatå UEXIT.MAC® 
Extracô iô froí LIBCREL.LBR¬ anä compilå it¬ theî replacå iô intï 
LIBC.REL:

     M80 =UEXIT 
     LIB80 NEW=LIBC<..UEXIT-1>,UEXIT,LIBC<UEXIT+1..>
     PIP LIBC.REL=NEW.REL

Thå  ideá ió tï maintaiî thå samå orderinç aó LIBC.REÌ  currentlù 
has®  Therå  maù  bå aî easieò approach¬  buô É havå prettù  mucè 
always done it this way.

An example is provided:

     C -S SAMPLE.C
     AS2MAC SAMPLE.AS >SAMPLE.MAC
     M80 =SAMPLE
     L80 ZCRTCPM,SAMPLE,SAMPLE/N/E
 