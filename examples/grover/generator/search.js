let search = (nqubits, targetState) => {
    let prog = "\n";
    let list = [];
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == "0") {
            list.push("P(q" + i + ", |0>)" )
        } else {
            list.push("P(q" + i + ", |1>)" )
        }
    }
    prog += "search in GROVER with =>! such that " + list.join(" and ") + " .";
    return prog;
}

module.exports = {search};