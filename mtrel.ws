..---!----!----!----!----!----!----!----!----!----!----!--------R
MTREL.WS

     Th� HITECȠ � compile� 3.0� appear� t� b� on� o� th� bes� �
choice� fo� loca� � developmen� unde� CP/M-8� (wit� � Z80)�� I� �
appear� t� generat� "reasonable� Z8� code�� ha� � reasonabl� run-�
tim� library� an� actuall� support� ANS� prototypes.

     However�� lik� man� simila� product� o� th� day�� i� style� �
itsel� a� � complet� "system"�� I� ha� � à compiler�� Assembler� �
an� Linker� Th� ide� i� tha� use� buy� int� th� whol� system.

     Sinc� HITEC� � (lik� Whitesmith� C� run� i� separat� passes� �
an� alway� produce� assembler� th� assemble� ca� b� munge� int� � �
forma�� suitabl� fo� anothe� system�� � chos� Microsof�� M80/L80� �
RE̠ file� ar� als� use� b� Digita� Researc� tool� lik� RMAC�� s� �
thi� give� u� mor� flexibility.

     The HITECH C driver (C.COM) can be run like:

     à -� HELLO.� (o� � -� -� HELLO.� i� th� peephol� optimize� �
                    is desired)

     Thi� produce� � HELLO.A� file� whic� ca� b� processe� b� th� �
HITEC� ZA� assembler� � generate� � utility:

     AS2MAC HELLO.AS >HELLO.MAC

     AS2MAà convert� a� A� fil� produce� b� � int� MAà forma� �
suitabl� fo� Microsof�� M80�� NOTE:� Th� '>�� characte� i� no� �
optional�� AS2MAà alway� write� th� outpu� t� th� consol� bu� �
redirectio� i� supported�� AS2MAC.à i� included�� Ther� i� � �
versio� insid� LIBCREL.LBҠ -- ignor� tha� on� (i�� i� ou�� o� �
date).

     Th� A� file� produce� b� HITEC� � hav� forwar� referenc� (j� �
1f..�� 1:� label� whic� M8� doesn'� support�� Als� psect�� .high� �
.low�� an� &�� I� th� -� (optimize� optio� i� used�� symbol� �
beginnin� wit� � ar� generate� an� conflic� wit� lower-cas� � �
symbol� (M8�� i� no� cas� sensitive)�� Floatin� poin�� (deff� �
constant� ar� replace� wit� fou� byt� sequences��  globa� i� �
converte� t� exter� i� needed�� Extende� conditio� code� ar� �
replace� (e� j� llt)�� T� accomplis� th� conversion� AS2MA� take� �
tw� passe� ove� th� A� file�� I� alway� write� th� fina� MAà t� �
stdout�� AS2MA� i� NO� � genera� utilit� -- i� ONL� convert� th� �
outpu� fro� C� Afte� th� conversion:

     M80 =HELLO

and link with:

     L80 ZCRTCPM,HELLO,HELLO/N/E

ɠ converte� th� � librar� t� LIBC.REL�� an� th� floatin� librar� �
t� LIBF.REL�� Not� tha� ZCRTCP� ha� � .REQUIR� LIBC�� I� floatin� �
point is used, LIBF/S must be added to the link.
�     Sinc堠 th� L8�� linke� onl�� understand� ��� significan� �
characters� th� followin� name� ar� change� (an� STRING.� fixed):

     strncpy -> stncpy
     strncat -> stncat
     strncmp -> stncmp

     Som� interna� name� ar� change� a� wel� -- th� update� �
librar� sourc� i� provided.

     UEXIT.MAà ha� bee� modifie� t� writ� th� retur� cod� t� �
locatio� 0x7e�� no� 0x8� a� th� origina� HiTec� cod� did� 0x8� i� �
destroye� b� th� CC� befor� th� nex� progra� i� executed� 0x7� i� �
safer.

     ɠ als� patche� th� drive� (C.COM� t� refe� t� LINQ.CO͠ an� �
CREQ.CO� t� avoi� problem� whe� combinin� wit� othe� tool-chains� �
LINK.COM is then named LINQ.COM, and CREF.COM is named CREQ.COM.

     Azte� à i� th� nex� mos� usefu� � compile� fo� m�� needs� �
But�� Azte� doe� no� offe� functio� prototypes� Whitesmith'� � i� �
wha� � use� "i� th� day"� bu� i� to� i� non-ANS� (n� prototypes)� �
ha� � non-standar� library�� � ver�� lock-i� toolchai� (th� �
assemble� i� somethin� calle� "A-natural")�� Whitesmith� als� �
take� great� (� 62K� TP� i� minimal� dea� o� memory�� an� i� 808� �
only�� Hi-Tec� operate� wit� CP/NE� 1.2�� whic� i� m�� preferre� �
environment�� Azte� generate� bot� 808� an� Z8� --  Hi-Tec� à i� �
Z8� only� Howeve� � ca� accep� tha� limitation.

     Fo� thos� reasons�� � selecte� Hi-Tec� a� m� � compile� o� �
choice�� However�� ɠ d� no� wan� t� leav� MACRO-80�� a� tha�� i� �
still� preferre� assemble� syste� o� CP/M�� Thu� th� MTRE� projec� �
wa� born.

     T� updat� � modul� i� � librar� wit� lib8� us� th� followin� �
approach�� Fo� example�� le� u� sa� yo� wan� t� updat� UEXIT.MAC� �
Extrac� i� fro� LIBCREL.LBR� an� compil� it� the� replac� i� int� �
LIBC.REL:

     M80 =UEXIT 
     LIB80 NEW=LIBC<..UEXIT-1>,UEXIT,LIBC<UEXIT+1..>
     PIP LIBC.REL=NEW.REL

Th� ide� i� t� maintai� th� sam� orderin� a� LIBC.RE̠ currentl� �
has�� Ther� ma�� b� a� easie� approach�� bu� � hav� prett�� muc� �
always done it this way.

An example is provided:

     C -S SAMPLE.C
     AS2MAC SAMPLE.AS >SAMPLE.MAC
     M80 =SAMPLE
     L80 ZCRTCPM,SAMPLE,SAMPLE/N/E
 