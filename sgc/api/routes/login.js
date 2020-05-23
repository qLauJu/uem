module.exports = app => {
    const loginControladora = app.controladoras.login;

    app.post('/login', loginControladora.login);
};