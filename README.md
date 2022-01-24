<div align="center">
  <a href="https://github.com/nicolas-stein/EcoCity">
    <img src="logo.png" alt="Logo" width="350" height="350">
  </a>
</div>

# EcoCity
## A propos du projet
Bonjour, vous trouverez dans ce dépôt toutes les instructions afin de compiler et jouer à notre jeu **EcoCity** . <br /><br />
Vous trouverez le rapport ici : [*Rapport_Stein_Roussarie.pdf*](https://github.com/nicolas-stein/EcoCity/blob/master/Rapport_Stein_Roussarie.pdf)<br />
Ainsi que le guide du jeu ici : [*Guide_de_jeu.pdf*](https://github.com/nicolas-stein/EcoCity/blob/master/Guide_de_jeu.pdf)<br />
Le diagramme UML complet et en haute définition se trouve également ici :  [*UML_EcoCity.png*](https://github.com/nicolas-stein/EcoCity/blob/master/UML_EcoCity.png) <br /><br />
Nous avons  beaucoup travaillé sur ce jeu alors nous espérons que vous l'apprécierez ! :smile:<br />

## Compilation

**Dans le cas où vous n'arriveriez pas à compiler le jeu, vous pouvez télécharger une version déjà compilée pour Linux ou Windows [ici](https://github.com/nicolas-stein/EcoCity/releases/tag/Release)**

### Prérequis
Le jeu utilise le framework Qt, plus précisement la version 6.2.2, il faudra donc l'installer afin de compiler le jeu.

Suivez ces instructions pour installer Qt :

1. Télécharger le [Qt Online Installer](https://www.qt.io/download-qt-installer "Qt Online Installer")

2. Lancer l'exécutable :<br />
 *Note : sous Linux il faudra rendre le fichier .run téléchargé exécutable<br />
  Pour cela, ouvrir un terminal dans le dossier de téléchargement puis exécuter la commande suivante : `chmod +x qt-unified-linux-x64-4.2.0-online.run`<br />
  Vous pourrez ensuite lancer l'installeur normalement*
  
3. Connectez vous avec un compte Qt, si vous n'en avez pas il est nécessaire d'en créer un.

4. Acceptez les obligations Open Source de Qt

5. Spécifiez le dossier dans lequel Qt va être installé et cochez "Custom installation"

6. Sélectionnez les composants nécessaire comme sur la capture d'écran si dessous :
    - Linux :<br />
    <a href="https://ibb.co/kgjG9pn"><img src="https://i.ibb.co/RpfgB1n/Capture-d-cran.png" alt="Capture-d-cran" border="0"></a>
    - Windows :<br />
    <a href="https://ibb.co/Q9pSm4y"><img src="https://i.ibb.co/4WKvNHh/Capture-d-cran.jpg" alt="Capture-d-cran" border="0"></a>
   <a href="https://ibb.co/Qf2SSLV"><img src="https://i.ibb.co/DtxcchF/Capture-d-cran.jpg" alt="Capture-d-cran" border="0"></a>
  
7. Acceptez les contrats de licence
8. Lancez l'installation



### Compilation du jeu

Pour compiler le jeu, suivre les étapes :

1. Clonez ou téléchargez le dépôt Github sur votre machine<br />
*Attention à ne pas mettre le dépôt dans des dossiers avec des accents*

2. Allez dans le dossier du dépôt puis éxecutez les commandes suivantes pour compiler le programme :<br />
    - Linux :<br />
    ```sh
    /chemin/vers/Qt/6.2.2/gcc_64/bin/qmake EcoCity.pro
    make release
    ```
    - Windows :<br />
    *Depuis une invite de commande (et non Powershell)*<br />
    ```bash
    C:\chemin\vers\Qt\6.2.2\mingw_64\bin\qtenv2.bat
    cd C:\chemin\vers\depot\
    qmake
    C:\chemin\vers\Qt\Tools\mingw900_64\bin\mingw32-make release
    move release\EcoCity.exe EcoCity.exe
    windeployqt EcoCity.exe
    ```
    Le programme doit se compiler correctement et un exécutable `EcoCity` doit avoir été créé.

3. Ensuite, compilez les ressources avec les commandes suivantes :<br />

    - Linux :
    ```sh
    /chemin/vers/Qt/6.2.2/gcc_64/libexec/rcc -binary ressources_audio.qrc -o ressources_audio.rcc
    /chemin/vers/Qt/6.2.2/gcc_64/libexec/rcc -binary ressources_graphics.qrc -o ressources_graphics.rcc
    /chemin/vers/Qt/6.2.2/gcc_64/libexec/rcc -binary ressources_others.qrc -o ressources_others.rcc
    ```
    
    - Windows :
    ```sh
    create_ressources_rcc.bat
    ```

4. Enfin, lancez le jeu :<br />
    ```sh
    ./EcoCity
    ```
    Ou double cliquez sur l'éxecutable EcoCity.exe

## Le jeu
Maintenant que vous avez lancé le jeu, vous pouvez y jouer autant que vous voulez !
N'hésitez pas à lire le guide de jeu afin de mieux comprendre son fonctionnement.
