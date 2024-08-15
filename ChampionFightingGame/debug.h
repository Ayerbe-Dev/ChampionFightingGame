#pragma once
//#define DEBUG

/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM0:.',;;;.....'...........'kMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMWXxd0WNKKNMMMMMMMMMMMMMMMMMMMMMMMMMMMXl.'',;;;,................:XMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMNNWMMNKOxxk0Odoool;..,c:;;lOWMMMMMMMMMMMMMMMMMMMMMMMMWx'.',,,,;,..'............'kWMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMKddxddl;;cc;....'..'''..';;:okKWMMMMMMMMMMMMMMMMMMMMM0:..',;;;;'...............dWMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMWN0O0Kx,.','.....''',;,'';;,'';kWMMMMMMMMMMMMMMMMMMMNd''..';;;,'..............:KMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMO,...........',;;;;;'.',';OWMMMMMMMMMMMMMMMMMMK:''...,;,...............'OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMNo...........''',;;,...'''lXMMMMMMMMMMMMMMMMMMK:.....','...  ..........dWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMXl.............',;,'....'oNMMMMMMMMMMMMMMMMMMWXx;.........  .........lNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMXl........  ...''..'...,OWMMMMMMMMMMMMMMMMMMW0d;..'................cXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMNd'....... ...........':o0WMMMMMMMMMMMMMMMNx,'''',,'.............:KMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMNk,............,,,,,'''.;xOkxddoloooodxxkx;...'..''............;0MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMW0:...........'''',''...'....................................,OWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMXo...................'.......................':o:.........;OWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMWx'..................'''''''',,'''...........'lc. .,lc:cxXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMW0c'.............'''',,;;;;;;;;;;,,,''.'.......  'kMWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMW0xl,.........'',,'';;;;;:::::::::;;;;,'..''....:0WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXo'....'.''',,,'.',;::::ccccccccc:::;;,'''.....;kWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd'........'',,,,;;;:::::::cccccccc::;;;,'''.....,OWWNNNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMWk,........'',,,,,;'':::;:;;:::cccc::;;;;,'.','....cdx0KKNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMW0:....''.....'...,;.';::::;;,'';;;'..'''''..',.......;OWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMK:.....'.............,;;;;:::;..................'......:dx0NWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMWx...........        ..,;;::::;'.          ......'......;odOKXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMXc.........      ...  .';;;;;:;.      ....   ............;oKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMO,..... ...  ...;lc,.  ...,;;,,.   ..';c:,.. ......'......lKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMWd...... ..  .,oodxo;'.   .,;;;;.....;:col:,.. ....'.......cKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMWKOd,....''......ldcll;''....';;;;,'...,::co:;;'. ....'......'xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMWNKKKOxdl,.....''.....,:cc:''',,,',;;;,,''...,:cc:;,......''......cXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMWXKXXX0ool,.....''.. ...'..''''''',;;,,'''''...''..........'......dWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMXdod'......'........................''''............'.... .,OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMWkoo;.......'....''.......................'''.''. ....... .oNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMWklkk,............................................. ..... ,OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMM0d0Xl.............'.................',,'.......... ......oNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMXk0Wk'...........','............  ..','.''......    ....cXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMNXWM0;..........'',,............ ..',,...''.....   ..  .cXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMWk;.........''''''..',,;;;,'..',,,'..........   ..  .,kWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMW0;........'.....'',,;;;;,,'',;,,'..  ......   ....;ONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMWo...............''',,;,,,'''''''.......     .....xWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO,..'..................................    .....cXMMMMMWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNl..............',,'.....'....  ...............,OWXOxolccclodk00OOOO0XNWMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMk,..... ............  ......  ...... .,;'.....lkc'.  .............'';:okKWMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNO:........',,...',''...,,,,.  .';;,...........'.    ..'''..... .........,l0WMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMWNXKKxxOc......''.'..',;;;'...',,.. ....'...........     .'',,,'..........   ...'oXMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMW0xc;,'....,;'......',,,''''........'.. ..'........  .     ...,,;;;'..'.........   ...cKMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMWXx:'..............   ....''.....''....'.........             ..',;;;,...'... ............lNMMMMMMMMMM
MMMMMMMMMMMMWWNXXKxcldxc............      .......................               ..'',,,...''''.... .....  ..,0MMMMMMMMMM
MMMMMMMMWKkdl:;,,';kWMNo'''.........     ..         ...                   .... ...'''''...'.......  ..    ..'kMMMMMMMMMM
MMMMMMNkc,........;kKKd;'',,'.......  .....                              .......''..............         ...,OMMMMMMMMMM
MMMMWO:....  ......''...',,,...............    ....           ..        .......''''...........      .    ...cXMMMMMMMMMM
MMMNd'....  ...  .......',,'.  ..............                            ......'''............           .  'dXMMMMMMMMM
MMNo.....  .'..  ......''','.   .............                             ....'........            .......   .oNMMMMMMMM
MNo......  ..'.   ........','...............                              ....'.......              ...      ,dxKWMMMMMM
Wk'......   ...     .........................                             ....'....                         .xN0xkXWMMMM
O, ......             .......................          ...              .....'.....     ...                 .lo;;;:odxKW
;   ........             ...    ............          ....  ...          ..........    ....;c.     ..          ....  .;O
.   ........                   ...........           ...........         ..........   .....cKx.   .c:.         ...     ,
*/