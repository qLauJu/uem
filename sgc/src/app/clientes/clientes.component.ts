import { Component, OnInit } from '@angular/core';
import { UsuariosService } from 'app/services/usuarios/usuarios.service';
import { Usuario } from 'app/models/usuario';

@Component({
  selector: 'app-clientes',
  templateUrl: './clientes.component.html',
  styleUrls: ['./clientes.component.css'],
  providers: [UsuariosService]
})
export class ClientesComponent implements OnInit {

  usuarios: Usuario[] = [];

  constructor(private usuariosService: UsuariosService) { }

  ngOnInit() {
    this.getUsuarios();
  }

  private getUsuarios(): void {
    this.usuariosService.getUsuarios()
      .then(usuarios => this.usuarios = usuarios)
      .catch(error => console.log(error));
  }

}
