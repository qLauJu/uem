import { Component, OnInit, Input, HostListener } from '@angular/core';
import { Router } from "@angular/router";
import { UsuariosService } from '../services/usuarios/usuarios.service';
import { Usuario } from '../models/usuario';

@Component({
  selector: 'app-criar-conta',
  templateUrl: './criar-conta.component.html',
  styleUrls: ['./criar-conta.component.css'],
  providers: [UsuariosService]
})
export class CriarContaComponent implements OnInit {

  @Input() usuario = new Usuario();
  repetirSenha: String;
  errorMessage: String;
  isLoading: Boolean = false;
  
  modalAberto: Boolean = false;
  
  constructor(private usuariosService: UsuariosService, private router: Router) { }

  register(): void {
    if (this.usuario.senha !== this.repetirSenha) {
      this.errorMessage = "As senhas não conferem."
      return;
    }

    this.isLoading = true;
    this.usuariosService.criarUsuario(this.usuario)
      .then(usuario => {
        this.isLoading = false;
        if (usuario) {
          this.router.navigate(['/login']);
        }
      })
      .catch(error => {
        this.isLoading = false;
        this.errorMessage = error
      });
  }

  abrirModal(): void {
    this.modalAberto = true;
  } 

 @HostListener('document:keydown', ['$event'])
  handleKeyEvent(event: KeyboardEvent): void {
    if (event.keyCode == 27) {
      this.esconderModal();
    }
  }

  esconderModal():void{
    this.modalAberto = false;
  }
  

  ngOnInit() {
  }

}
