import { SgcPage } from './app.po';

describe('sgc App', () => {
  let page: SgcPage;

  beforeEach(() => {
    page = new SgcPage();
  });

  it('should display welcome message', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('Welcome to app!');
  });
});
