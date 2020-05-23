import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CadastroCacambaComponent } from './cadastro-cacamba.component';

describe('CadastroCacambaComponent', () => {
  let component: CadastroCacambaComponent;
  let fixture: ComponentFixture<CadastroCacambaComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CadastroCacambaComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CadastroCacambaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
