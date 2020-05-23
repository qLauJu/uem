import { Component, OnInit, Input } from '@angular/core';
import { AuthService } from '../services/auth/auth.service';
import { Router } from '@angular/router';
import { Usuario } from '../models/usuario';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  @Input() usuario = new Usuario();
  errorMessage: string;
  isLoading = false;

  constructor(private authService: AuthService, private router: Router) { }

  login(): void {
    this.isLoading = true;
    this.authService.autenticarUsuario(this.usuario)
      .then(response => {
        this.isLoading = false;
        if (response.token) {
          this.router.navigate(['painel']);
        }
      }).catch(error => {
        this.isLoading = false;
        this.errorMessage = error;
      });
  }

  ngOnInit() {
  }

}
