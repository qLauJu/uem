module.exports = app => {
    const pedidosController = app.controladoras.pedidos;

    app.route('/pedidos')
        .all(app.auth.authenticate())
        .get((req, res) => {
            if (req.user.tipo === "GERENTE") {
                pedidosController.listarPedidosGerente(req, res);
            } else {
                pedidosController.listarPedidosCliente(req, res);
            }
        })
        .post((req, res) => {
            if (req.user.tipo === "GERENTE") {
                pedidosController.criarPedidoGerente(req, res);
            } else {
                pedidosController.criarPedidoCliente(req, res);
            }
        });

    app.route('/pedidos/:id')
        .all(app.auth.authenticate())
        .put(pedidosController.alterarPedido)
        .delete(app.auth.authorize("GERENTE"), pedidosController.removerPedido);

    app.route('/pedidos/calcular')
        .all(app.auth.authenticate())
        .post(pedidosController.calcularPedido);

    app.route('/pedidos/status')
        .all(app.auth.authenticate())
        .get(pedidosController.listarStatus);
}