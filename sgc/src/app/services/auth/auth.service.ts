import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import { BaseService } from '../base/base.service';
import { Usuario } from '../../models/usuario';
import { LoginResponse } from '../response-models/login-response';

@Injectable()
export class AuthService extends BaseService {
    constructor(private http: Http) {
        super(http);
    }

    autenticarUsuario(usuario: Usuario): Promise<LoginResponse> {
        return this.http.post(this.baseUrl + '/login', usuario).toPromise()
            .then(response => {
                const result = response.json() as LoginResponse;
                localStorage.setItem('usuarioToken', result.token);
                localStorage.setItem('usuarioTipo', result.tipo);
                return result;
            })
            .catch(this.handleError);
    }

    isLoggedIn(): boolean {
        return localStorage.getItem('usuarioToken') !== null;
    }

    getToken(): string {
        return localStorage.getItem('usuarioToken');
    }

    getTipoUsuario(): string {
        return localStorage.getItem('usuarioTipo');
    }

    logOut(): void {
        localStorage.removeItem('usuarioToken');
        localStorage.removeItem('usuarioTipo');
    }
}
