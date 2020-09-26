# khangstuffau
experimental patches for among us

this project would not have been possible without the amazing [Il2CppDumper](https://github.com/Perfare/Il2CppDumper) project!

also uses [inih](https://github.com/benhoyt/inih) and [QuickHook](https://github.com/CPunch/QuickHook)

## Installation
1. download a release or compile this yourself (x86/Release) and make sure it's actually compatible with your version of among us
2. use [Stud_PE](http://www.cgsoftlabs.ro/studpe.html) to add the dummy import to the GameAssembly dll
    * open it
    * drag the GameAssembly dll onto it
    * click the functions tab
    * right click the dll list
    * add new import
    * select the dll
    * add dummyexport to the list
3. copy the dll over to your among us installation (if you compile it yourself, it automatically copies the dll to your steam directory for debugging)
4. install hats

## Configuration
you have to make a config.ini file in the khangstuffau folder, as this won't make it itself

it runs perfectly fine without one though

current allowed config options are:
```
unlock_framerate=false ; unlocks your framerate if you don't have vsync on
```

## Hat Installation/Creation
copy the hats into the folder like so
```
├───khangstuffau
│   │   config.ini
│   │
│   └───hats
│           test-back.png
│           test.ini
│           troll-front.png
│           troll.ini
```
for hat creation, the only documentation for now is the example in the repo
