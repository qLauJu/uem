module.exports = app => {

    const painelControladora = app.controladoras.painel;

    app.route('/painel')
        .all(app.auth.authenticate())
        .get((req, res) => {
            if (req.user.tipo === 'GERENTE') {
                painelControladora.getPainelGerente(req, res);
            } else {
                painelControladora.getPainelCliente(req, res);
            }
        });

}