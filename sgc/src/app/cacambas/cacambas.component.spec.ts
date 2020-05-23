import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CacambasComponent } from './cacambas.component';

describe('CacambasComponent', () => {
  let component: CacambasComponent;
  let fixture: ComponentFixture<CacambasComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CacambasComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CacambasComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
