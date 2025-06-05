let pcheck = (nqubits, enable=true) => {
    let prog = "\n";
    if (!enable) {
        prog += "// ";
    }
    prog += "pcheck in GROVER with 'P=? [F \"target\"]' --backend=Storm .";
    return prog;
}

module.exports = {pcheck};