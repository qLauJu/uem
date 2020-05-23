import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/toPromise';
import { BaseService } from '../base/base.service';
import { Usuario } from '../../models/usuario';

@Injectable()
export class UsuariosService extends BaseService {

  constructor(private http: Http) {
    super(http);
  }

  criarUsuario(usuario: Usuario): Promise<Usuario> {
    return this.http.post(this.baseUrl + '/usuarios', usuario).toPromise()
      .then(response => response.json() as Usuario)
      .catch(this.handleError);
  }

  getUsuarios(): Promise<Usuario[]> {
    return this.http.get(this.baseUrl + '/usuarios', { headers: this.headers }).toPromise()
      .then(response => response.json() as Usuario[])
      .catch(this.handleError);
  }

  getDocumentos(): Promise<string[]> {
    return this.http.get(this.baseUrl + '/usuarios/documentos', { headers: this.headers }).toPromise()
      .then(response => response.json() as string[])
      .catch(this.handleError);
  }

}
