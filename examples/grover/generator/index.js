const fs = require('fs');
const {grover} = require("./grover.js");
const {nqubits, targetState, nIters} = require("./global.js");

const content = grover(nqubits, targetState, nIters, searchCmd = false, pcheckCmd = true);
const filePath = 'grover.qw';
fs.writeFile(filePath, content, err => {
    if (err) {
        console.error(err);
    } else {
        console.log('grover.qw is generated successfully');
    }
});