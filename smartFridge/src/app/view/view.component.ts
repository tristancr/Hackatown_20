import { Component, OnInit, Input } from '@angular/core';
import { Recipe } from './recipe';
import { HttpClient } from '@angular/common/http';
import { Product } from './product';
const URLFIRSTPART = 'https://api.edamam.com/search?q=';
const URLSECONDPART = '&app_id=a75732ea&app_key=a7f69a13f95037e6c8cbe8840b5d7a99';
@Component({
  selector: 'app-view',
  templateUrl: './view.component.html',
  styleUrls: ['./view.component.css']
})
export class ViewComponent implements OnInit {
  private productArray: Product[];
  private productToCookArray: Product[];
  @Input() newProductName: string;
  @Input() newProductExpirationDate: Date;
  private recipeArray: Recipe[];
  private viewProducts: number;
  private viewRecipe: number;
  protected stringProducts: string[];
  protected stringRecipe: string[];
  protected stringButton: string[];
  constructor(private http: HttpClient) {
    this.recipeArray = new Array();
    this.productArray = new Array();
    this.productToCookArray = new Array();
    this.viewProducts = 0;
    this.viewRecipe = 0;
    this.stringProducts = new Array();
    this.stringProducts.push('see products');
    this.stringProducts.push('hide products');
    this.stringRecipe = new Array();
    this.stringRecipe.push('see recipes');
    this.stringRecipe.push('hide recipes');
    this.stringButton = new Array();
    this.stringButton.push('add');
    this.stringButton.push('added');
  }
  ngOnInit() {
  }

  goToRecipe(url: string) {
    location.href = url;
  }

  addProduct() {
    this.productArray.push({
      name: this.newProductName,
      expirationDate: this.newProductExpirationDate,
      isAdded: false,
    });
    this.newProductName = '';
  }

  changeProduct() {
    this.viewProducts = this.viewProducts === 0 ? 1 : 0;
  }

  changeButton(index: number) {
    if (this.productArray[index].isAdded) {
      this.productToCookArray.splice(this.productToCookArray.indexOf(this.productArray[index]), 1);
      this.productArray[index].isAdded = false;
    } else {
      this.productToCookArray.push(this.productArray[index]);
      this.productArray[index].isAdded = true;
    }
    this.findRecipe();
  }

  remove(index: number) {
    this.productArray.splice(index, 1);
  }

  changeRecipe() {
    this.viewRecipe = this.viewRecipe === 0 ? 1 : 0;
    this.findRecipe();
  }

  calculateTimeLeft(index: number): any {
    const date1 = new Date (this.productArray[index].expirationDate);
    const date2 = new Date();
    const timeDiff = Math.abs(date2.getTime() - date1.getTime());
    return Math.ceil(timeDiff / (1000 * 3600 * 24));
  }

  productsToString(): string {
    let stringProducts = this.productToCookArray[0].name;
    for (let index = 1; index < this.productToCookArray.length; index++) {
      stringProducts += '+' + this.productToCookArray[index];
    }
    return stringProducts;
  }
  findRecipe() {
    // tslint:disable-next-line:max-line-length
    this.http.get(URLFIRSTPART + this.productsToString() + URLSECONDPART)
      .subscribe((response: any) => {
        console.log(response);
        this.recipeArray = [];
        for (const currentRecipe of response.hits) {
          this.recipeArray.push( {
            nom: currentRecipe.recipe.label,
            image: currentRecipe.recipe.image,
            url: currentRecipe.recipe.url
          });
        }
        // response.hits[0]
        console.log(this.recipeArray);
      });
  }
}
