let atomicProps = (nqubits, targetState) => {
    prog = "prop\n";
    let list = [];
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == "0") {
            list.push("P(q" + i + ", |0>)" )
        } else {
            list.push("P(q" + i + ", |1>)" )
        }
    }
    prog += "\ttarget := " + list.join(" and ") + ";\n";
    return prog;
}

module.exports = {atomicProps};