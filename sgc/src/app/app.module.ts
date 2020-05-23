import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { NguiAutoCompleteModule } from '@ngui/auto-complete';
import { CalendarModule } from 'primeng/primeng';

import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { BarraLateralComponent } from './barraLateral/barraLateral.component';
import { BarraNavegacaoComponent } from './barraNavegacao/barraNavegacao.component';
import { BotaoflutuanteComponent } from './botaoflutuante/botaoflutuante.component';
import { AuthService } from './services/auth/auth.service';
import { LoginComponent } from './login/login.component';
import { HttpModule } from '@angular/http';
import { ClientesComponent } from './clientes/clientes.component';
import { PedidosComponent } from './pedidos/pedidos.component';
import { CadastroPedidoComponent } from './cadastro-pedido/cadastro-pedido.component';
import { PainelComponent } from './painel/painel.component';
import { CacambasComponent } from './cacambas/cacambas.component';
import { CadastroCacambaComponent } from './cadastro-cacamba/cadastro-cacamba.component';
import { CriarContaComponent } from './criar-conta/criar-conta.component';
import { CadastroClienteComponent } from './cadastro-cliente/cadastro-cliente.component';

@NgModule({
  declarations: [
    AppComponent,
    BarraLateralComponent,
    BarraNavegacaoComponent,
    BotaoflutuanteComponent,
    LoginComponent,
    ClientesComponent,
    PedidosComponent,
    CadastroPedidoComponent,
    PainelComponent,
    CacambasComponent,
    CadastroCacambaComponent,
    CriarContaComponent,
    CadastroClienteComponent
  ],
  imports: [
    BrowserModule,
    HttpModule,
    FormsModule,
    AppRoutingModule,
    ReactiveFormsModule,
    NguiAutoCompleteModule,
    CalendarModule,
    BrowserAnimationsModule
  ],
  providers: [AuthService],
  bootstrap: [AppComponent]
})
export class AppModule { }
