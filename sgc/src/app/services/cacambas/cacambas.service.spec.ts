import { TestBed, inject } from '@angular/core/testing';

import { CacambasService } from './cacambas.service';

describe('CacambasService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [CacambasService]
    });
  });

  it('should ...', inject([CacambasService], (service: CacambasService) => {
    expect(service).toBeTruthy();
  }));
});
