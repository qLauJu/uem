import { Router } from '@angular/router';
import { AuthService } from './../services/auth/auth.service';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-navbar',
  templateUrl: './barraNavegacao.component.html',
  styleUrls: ['./barraNavegacao.component.css']
})
export class BarraNavegacaoComponent implements OnInit {

  constructor(private authService: AuthService, private router: Router) { }

  ngOnInit() { }

  logOut(): void {
    this.authService.logOut();
    this.router.navigate(['/login']);
  }

}
