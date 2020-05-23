import { Injectable } from '@angular/core';
import { Headers, Http } from '@angular/http';


import 'rxjs/add/operator/toPromise';
import { environment } from '../../../environments/environment';
import { DefaultResponse } from '../response-models/default-response';

@Injectable()
export class BaseService {
  protected baseUrl: string;
  protected headers = new Headers({
    'Content-Type': 'application/json',
    'Authorization': `Bearer ${localStorage.getItem('usuarioToken')}`,
    'Accept': 'application/json'
  });

  constructor(http: Http) {
    this.baseUrl = environment.apiUrl;
  }

  protected handleError(error: Response | any): Promise<any> {
    return Promise.reject((error.json() as DefaultResponse).mensagem || 'Um erro inexperado aconteceu, por favor tente novamente.');
  }

}
