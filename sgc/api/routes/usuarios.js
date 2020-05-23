module.exports = app => {
    const usuariosControladora = app.controladoras.usuarios;

    app.route('/usuarios')
        .get(app.auth.authenticate(), app.auth.authorize("GERENTE"), usuariosControladora.getUsuarios)
        .post(usuariosControladora.inserirUsuario);

    app.get("/usuarios/documentos",
        app.auth.authenticate(),
        app.auth.authorize("GERENTE"),
        usuariosControladora.getDocumentos);

    app.route("/usuarios/:id")
        .all(app.auth.authenticate(), app.auth.authorize("GERENTE"))
        .get(usuariosControladora.getUsuario);

    app.route('/usuario')
        .all(app.auth.authenticate())
        .get(usuariosControladora.getPerfil)
        .delete(usuariosControladora.removerUsuario);    
};
