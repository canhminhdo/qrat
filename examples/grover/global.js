targetState5  = "01101";
targetState10 = "0110101101";
targetState15 = "011010110101011";
targetState20 = "01101011010101101101";
targetState = targetState20;
nqubits = targetState.length;
nIters = Math.floor(Math.PI / 4 * Math.sqrt(Math.pow(2, nqubits)));

module.exports = {nqubits, targetState, nIters};