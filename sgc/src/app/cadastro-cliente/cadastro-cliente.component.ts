import { Component, OnInit, Input } from '@angular/core';
import { Usuario } from '../models/usuario';
import { UsuariosService } from '../services/usuarios/usuarios.service';

@Component({
  selector: 'app-cadastro-cliente',
  templateUrl: './cadastro-cliente.component.html',
  styleUrls: ['./cadastro-cliente.component.css'],
  providers: [UsuariosService]
})
export class CadastroClienteComponent implements OnInit {

  @Input() usuario = new Usuario();
  carregando = false;
  constructor(private usuarioService: UsuariosService) { }

  ngOnInit() {
  }

  criar(): void {
    this.carregando = true;
    this.usuario.senha = this.usuario.documento;
    this.usuarioService.criarUsuario(this.usuario)
      .then(response => {
        this.carregando = false;
        console.log(response);
      })
      .catch(error => {
        this.carregando = false;
        console.log(error);
      });
  }
}
