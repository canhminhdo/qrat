// https://www.quantum-inspire.com/kbase/grover-algorithm/

const {header} = require("./header.js");
const {init} = require("./init.js");
const {oracle} = require("./oracle.js");
const {diffusion} = require("./diffusion.js");
const {measure} = require("./measure.js");
const {search} = require("./search.js");

function grover(nqubits, targetState, nIters) {
    let headProg = header(nqubits);
    let initProg = init(nqubits);
    let oracleProg = oracle(nqubits, targetState);
    let diffusionProg = diffusion(nqubits);
    let searchProg = search(nqubits, targetState);
    let prog = "// #qubit: " + nqubits + ", marked state: " + targetState.split('').reverse().join('') + ", #iterations: " + nIters + "\n";
    prog += headProg;
    prog += "begin\n";
    prog += initProg;
    for (i = 0; i < nIters; i++) {
        prog += oracleProg + diffusionProg;
    }
    prog += measure(nqubits);
    prog += "end\n"
    prog += searchProg;
    return prog;
}

module.exports = {grover};