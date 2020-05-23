import jwt from 'jwt-simple';

describe('Rota: Usuarios', () => {
    const Usuario = app.db.models.Usuario;

    describe('POST /usuarios', () => {
        beforeEach(done => {
            Usuario.destroy({where: {}}).then(done());
        });

        describe('status 200', () => {
            it('não retorna campo senha', done => {
                request.post('/usuarios')
                    .send({
                        documento: 'admin',
                        senha: 'admin',
                        tipo: 'GERENTE'
                    })
                    .expect(200)
                    .end((err, res) => {
                        expect(res.body).to.not.include.keys('senha');
                        done(err);
                    });
            });

            it('retorna usuário do tipo gerente', done => {
                request.post('/usuarios')
                    .send({
                        documento: 'admin',
                        senha: 'admin',
                        tipo: 'GERENTE'
                    })
                    .expect(200)
                    .end((err, res) => {
                        expect(res.body.tipo).to.eql('GERENTE');
                        expect(res.body.documento).to.eql('admin');
                        done(err);
                    });
            });
        });
    });
});