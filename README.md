# dictionary_webapp
A simple webapp which will implement a spell checker. Back end is by a c/cpp code.

How to run the code?

Make sure you have Node.js, npm and node-gyp tools installed. Look up online on how to install them, installing node-gyp can be 
tricky on Windows, Good Luck!
Prepare the Add-On module, each add-on module has to be installed specific to the OS platform.
Command : node-gyp build configure

Next, Install the Node Modules required, binding.gyp includes the necessary information, those are 'Express' & 'Morgan'
Command : npm install

You're all setup. Now run the command to run the server.js file in Node.
Command : node server.js
