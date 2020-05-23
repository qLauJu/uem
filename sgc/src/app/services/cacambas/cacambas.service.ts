import { Injectable } from '@angular/core';
import { BaseService } from '../base/base.service';
import { Cacamba } from '../../models/cacamba';
import { Http } from '@angular/http';
import { DefaultResponse } from '../response-models/default-response';

@Injectable()
export class CacambasService extends BaseService {

  constructor(private http: Http) {
    super(http);
  }

  criarCacamba(cacamba: Cacamba): Promise<Cacamba> {
    return this.http.post(this.baseUrl + '/cacambas', cacamba, {headers: this.headers}).toPromise()
      .then(response => response.json() as Cacamba)
      .catch(this.handleError);
  }

  getCacambas(): Promise<Cacamba[]> {
    return this.http.get(this.baseUrl + '/cacambas', {headers: this.headers}).toPromise()
      .then(response => response.json() as Cacamba[])
      .catch(this.handleError);
  }

  editarCacamba(cacamba: Cacamba): Promise<DefaultResponse> {
    return this.http.put(this.baseUrl + `/cacambas/${cacamba.id}`, cacamba, {headers: this.headers}).toPromise()
      .then(response => response.json() as DefaultResponse)
      .catch(this.handleError);
  }

  removerCacamba(id: number): Promise<DefaultResponse> {
    return this.http.delete(this.baseUrl + `/cacambas/${id}`, {headers: this.headers}).toPromise()
      .then(response => response.json() as DefaultResponse)
      .catch(this.handleError);
  }

}
