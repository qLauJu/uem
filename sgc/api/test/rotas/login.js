describe('Routes: Login', () => {
    const Usuario = app.db.models.Usuario;

    describe('POST /login', () => {
        beforeEach(done => {
            Usuario.destroy({ where: {} })
                .then(() => Usuario.create({
                    documento: "12345",
                    senha: "cliente"
                }))
                .then(done());
        });

        describe('status 401', () => {
            it('lança um erro quando a senha é incorreta', done => {
                request.post('/login')
                    .send({
                        documento: '12345',
                        senha: 'SENHA_ERRADA'
                    })
                    .expect(401)
                    .end((err, res) => done(err));
            });

            it('lanca um erro quando o documeto não existe', done => {
                request.post('/login')
                    .send({
                        documento: 'DOCUMENTO_ERRADO',
                        senha: 'SENHA_ERRADA'
                    })
                    .expect(401)
                    .end((err, res) => done(err));
            });

            it('lança um erro quando o documento e senha não são enviados', done => {
                request.post('/login')
                    .expect(412)
                    .end((err, res) => done(err));
            });
        });

        describe('status 200', () => {
            it('retorna o token do usuário autenticado e seu tipo', done => {
                request.post('/login')
                    .send({
                        documento: '12345',
                        senha: 'cliente'
                    })
                    .expect(200)
                    .end((err, res) => {
                        expect(res.body).to.include.keys('token');
                        expect(res.body).to.include.keys('tipo');
                        done(err);
                    });
            });
        })
    });
});