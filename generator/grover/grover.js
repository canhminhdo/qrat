// https://www.quantum-inspire.com/kbase/grover-algorithm/

const fs = require('fs');
const {nqubits, targetState, nIters} = require("./global.js");
const {header} = require("./header.js");
const {init} = require("./init.js");
const {oracle} = require("./oracle.js");
const {diffusion} = require("./diffusion.js");
const {measure} = require("./measure.js");
const {search} = require("./search.js");

function grover(iterations) {
    let headProg = header();
    let initProg = init();
    let oracleProg = oracle();
    let diffusionProg = diffusion();
    let searchProg = search();
    let prog = "// #qubit: " + nqubits + ", marked state: " + targetState.split('').reverse().join('') + ", #iterations: " + nIters + "\n";
    prog += headProg;
    prog += "begin\n";
    prog += initProg;
    for (i = 0; i < iterations; i++) {
        prog += oracleProg + diffusionProg;
    }
    prog += measure();
    prog += "end\n"
    prog += searchProg;
    return prog;
}

const content = grover(nIters);
fs.writeFile('grover.qw', content, err => {
    if (err) {
      console.error(err);
    } else {
      console.log("generate `grover` program successfully");
    }
});
