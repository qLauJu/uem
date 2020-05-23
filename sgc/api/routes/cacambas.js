module.exports = app => {
    const controladoraCacambas = app.controladoras.cacambas;

    app.route('/cacambas')
        .all(app.auth.authenticate())
        .get(controladoraCacambas.getAll)
        .post(app.auth.authorize("GERENTE"), controladoraCacambas.criarCacamba);

    app.route('/cacambas/:id')
        .all(app.auth.authenticate(), app.auth.authorize("GERENTE"))
        .put(controladoraCacambas.editarCacamba)
        .delete(controladoraCacambas.removerCacamba);
}
