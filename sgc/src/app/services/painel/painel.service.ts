import { Injectable } from '@angular/core';
import { BaseService } from '../base/base.service';
import { Http } from '@angular/http';
import { PainelResponse } from '../response-models/painel-response';

@Injectable()
export class PainelService extends BaseService {

  constructor(private http: Http) {
    super(http);
   }

  getPainel(): Promise<PainelResponse> {
    return this.http.get(this.baseUrl + '/painel', {headers: this.headers}).toPromise()
      .then(response => response.json() as PainelResponse)
      .catch(this.handleError);
  }

}
