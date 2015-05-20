//
//  BUYDataProvider.h
//  Checkout
//
//  Created by Shopify.
//  Copyright (c) 2015 Shopify Inc. All rights reserved.
//

@import Foundation;
@import PassKit;
#import "BUYSerializable.h"

@class BUYCart;
@class BUYCheckout;
@class BUYCreditCard;
@class BUYGiftCard;
@class BUYProduct;
@class BUYProductVariant;
@class BUYShop;

/**
 *  A BUYStatus is associated with the completion of an enqueued job on Shopify.
 *  BUYStatus is equal is HTTP status codes returned from the server
 */
typedef NS_ENUM(NSUInteger, BUYStatus) {
	/**
	 *  The job is complete
	 */
	BUYStatusComplete = 200,
	/**
	 *  The job is still processing
	 */
	BUYStatusProcessing = 202,
	/**
	 *  The job is not found, please check the identifier
	 */
	BUYStatusNotFound = 404,
	/**
	 *  The precondition given in one or more of the request-header fields evaluated to false when it was tested on the server.
	 */
	BUYStatusPreconditionFailed = 412,
	/**
	 *  The request failed, refer to an NSError for details
	 */
	BUYStatusFailed = 424,
	/**
	 *  The status is unknown
	 */
	BUYStatusUnknown
};

/**
 *  Return block containing a BUYCheckout, Payment Session ID and/or an NSError
 *
 *  @param checkout         The returned BUYCheckout
 *  @param paymentSessionId The Payment Session ID associated with a credit card transaction
 *  @param error            Optional NSError
 */
typedef void (^BUYDataCreditCardBlock)(BUYCheckout *checkout, NSString *paymentSessionId, NSError *error);

/**
 *  Return block containing a BUYCheckout and/or an NSError
 *
 *  @param checkout The returned BUYCheckout
 *  @param error    Optional NSError
 */
typedef void (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);

/**
 *  Return block containing a BUYCheckout, a BUYStatus and/or an NSError
 *
 *  @param checkout The returned BUYCheckout
 *  @param status   A BUYStatus specifying the requested job's completion status
 *  @param error    Optional NSError
 */
typedef void (^BUYDataCheckoutStatusBlock)(BUYCheckout *checkout, BUYStatus status, NSError *error);

/**
 *  Return block containing BUYShippingRate objects, a BUYStatus and/or an NSError
 *
 *  @param shippingRates Array of SHKShippingRates
 *  @param status        A BUYStatus specifying the requested job's completion status
 *  @param error         Optional NSError
 */
typedef void (^BUYDataShippingRatesBlock)(NSArray *shippingRates, BUYStatus status, NSError *error);

/**
 *  Return block containing a BUYShop and/or an NSError
 *
 *  @param shop  A BUYShop object
 *  @param error Optional NSError
 */
typedef void (^BUYDataShopBlock)(BUYShop *shop, NSError *error);

/**
 *  Return block containing a BUYProduct and/or an NSError
 *
 *  @param product A BUYProduct
 *  @param error   Optional NSError
 */
typedef void (^BUYDataProductBlock)(BUYProduct *product, NSError *error);

/**
 *  Return block containing a list of BUYProduct objects, the page requested, a boolean to determine whether the end of the list has been reach and/or an optional NSError
 *
 *  @param products   An array of BUYProduct objects
 *  @param page       Index of the page requested
 *  @param reachedEnd Boolean indicating whether additional pages exist
 *  @param error      An optional NSError
 */
typedef void (^BUYDataProductListBlock)(NSArray *products, NSUInteger page, BOOL reachedEnd, NSError *error);

/**
 *  Return block containing a list of BUYProductImage objects and/or an NSError
 *
 *  @param images An array of BUYProductImage objects
 *  @param error  An optional NSError
 */
typedef void (^BUYDataImagesListBlock)(NSArray *images, NSError *error);

/**
 *  Return block containing a BUYGiftCard
 *
 *  @param giftCard A BUYGiftCard
 *  @param error    An optional NSError
 */
typedef void (^BUYDataGiftCardBlock)(BUYGiftCard *giftCard, NSError *error);

/**
 The BUYDataProvider provides all requests needed to perform request on the Shopify Checkout API.
 Use this class to perform tasks such as getting a shop, products for a shop, creating a Checkout on Shopify
 and completing Checkouts.
 */
@interface BUYClient : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 *  Initialize a BUYDataProvider using a shop's domain, API key and the Channel ID.
 *
 *  @param shopDomain The Shop Domain i.e. abetterlookingshop.myshopify.com
 *  @param apiKey     The API key provided via the Mobile SDK Channel on Shopify Admin
 *  @param channelId  The Channel ID provided on Shopify Admin
 *
 *  @return An instance of BUYDataProvider
 */
- (instancetype)initWithShopDomain:(NSString *)shopDomain apiKey:(NSString *)apiKey channelId:(NSString *)channelId NS_DESIGNATED_INITIALIZER;

/**
 *  Enable Apple Pay by calling this method with the Merchant ID provided via Apple Pay setup in the Mobile SDK Channel on Shopify Admin
 *
 *  @param merchantId The Merchant ID generated on Shopify Admin
 */
- (void)enableApplePayWithMerchantId:(NSString *)merchantId;

/**
 *  Test the integration with your shop.  This should not be shipped in production code
 *
 *  @return YES on success
 */
- (BOOL)testIntegration;

/**
 *  The page size for any paged request. This can range from 1-250.
 */
@property (nonatomic, assign) NSUInteger pageSize;

/**
 *  The shop domain set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *shopDomain;

/**
 *  The API Key set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *apiKey;

/**
 *  The Channel ID set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *channelId;

/**
 *  The Merchant ID is used for Apple Pay and set using `enableApplePayWithMerchantId:`
 */
@property (nonatomic, strong, readonly) NSString *merchantId;

/**
 *  Application name to attribute orders to.  Defaults to app bundle name (CFBundleName)
 */
@property (nonatomic, strong) NSString *applicationName;

#pragma mark - Storefront

/**
 *  Fetches the shop's metadata (from /meta.json).
 *
 *  @param block (^BUYDataShopBlock)(BUYShop *shop, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getShop:(BUYDataShopBlock)block;

/**
 *  Fetches a single page of products for the shop.
 *
 *  @param page  Page to request. Pages start at 1.
 *  @param block (^BUYDataProductListBlock)(NSArray *products, NSUInteger page, BOOL reachedEnd, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getProductsPage:(NSUInteger)page completion:(BUYDataProductListBlock)block;

/**
 *  Fetches a single product by the ID of the product.
 *
 *  @param prodcutId Product ID
 *  @param block  (^BUYDataProductBlock)(BUYProduct *product, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getProductById:(NSString *)productId completion:(BUYDataProductBlock)block;

#pragma mark - Checkout

/**
 *  Builds a checkout on Shopify. The checkout object is used to prepare an order
 *
 *  @param checkout BUYCheckout to create on Shopify
 *  @param block    (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)createCheckout:(BUYCheckout *)checkout completion:(BUYDataCheckoutBlock)block;

/**
 *  Builds a checkout on Shopify using a Cart Token from an existing cart on your Shopify store's storefront.
 *  The BUYCheckout object is used to prepare an order.
 *
 *  @param cartToken Cart Token associated with an existing BUYCheckout on Shopify
 *  @param block     (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)createCheckoutWithCartToken:(NSString *)cartToken completion:(BUYDataCheckoutBlock)block;

/**
 *  Applies a gift card code to the checkout.
 *
 *  @param giftCardCode The gift card code to apply on an existing checkout on Shopify. Note: This is not the same as the gift card identifier.
 *  @param checkout     An existing BUYCheckout on Shopify
 *  @param block        (^BUYDataGiftCardBlock)(BUYGiftCard *giftCard, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)applyGiftCardWithCode:(NSString *)giftCardCode toCheckout:(BUYCheckout *)checkout completion:(BUYDataGiftCardBlock)block;

/**
 *  Removes a gift card from the checkout.
 *
 *  @param giftCard The BUYGiftCard to remove
 *  @param checkout The BUYCheckout for which the Gift Card should be removed
 *  @param block    (^BUYDataGiftCardBlock)(BUYGiftCard *giftCard, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)removeGiftCard:(BUYGiftCard *)giftCard fromCheckout:(BUYCheckout *)checkout completion:(BUYDataGiftCardBlock)block;

/**
 *  Retrieves an updated version of a BUYCheckout from Shopify.
 *
 *  Note: There's no guarantee that the BUYCheckout returned will be the same as the one that is passed in.
 *  We recommended using the BUYCheckout returned in the block.
 *
 *  @param checkout The BUYCheckout to retrieve (updated) from Shopify
 *  @param block    (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getCheckout:(BUYCheckout *)checkout completion:(BUYDataCheckoutBlock)block;

/**
 *  Updates a given BUYCheckout on Shopify.
 *
 *  Note: There's no guarantee that the BUYCheckout returned will be the same as the one that is passed in.
 *  We recommended using the BUYCheckout returned in the block.
 *
 *  @param checkout The BUYCheckout to updated on Shopify
 *  @param block    (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)updateCheckout:(BUYCheckout *)checkout completion:(BUYDataCheckoutBlock)block;

/**
 *  Finalizes the BUYCheckout and charges the credit card.
 *  This enqueues a completion job on Shopify and returns immediately.
 *  You must get the job's status by calling checkCompletionStatusOfCheckout:block
 *
 *  Note: There's no guarantee that the BUYCheckout returned will be the same as the one that is passed in.
 *  We recommended using the BUYCheckout returned in the block.
 *
 *  @param checkout The BUYCheckout to complete
 *  @param block    (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)completeCheckout:(BUYCheckout *)checkout completion:(BUYDataCheckoutBlock)block;

/**
 */
/**
 *  Finalizes the checkout and charges the credit card associated with the payment token from PassKit (Apple Pay).
 *  This only enqueues a completion job, and will return immediately.
 *  You must get the job's status by calling checkCompletionStatusOfCheckout:block
 *
 *  Note: There's no guarantee that the BUYCheckout returned will be the same as the one that is passed in.
 *  We recommended using the BUYCheckout returned in the block.
 *
 *  @param checkout The BUYCheckout to complete
 *  @param token    The PKPaymentToken
 *  @param block    (^BUYDataCheckoutBlock)(BUYCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)completeCheckout:(BUYCheckout *)checkout withApplePayToken:(PKPaymentToken *)token completion:(BUYDataCheckoutBlock)block;

/**
 *  Retrieve the status of a BUYCheckout. This checks the status of the current payment processing job for the provided checkout.
 *  Once the job is complete (status == BUYStatusComplete), you can use the `orderId` property on BUYCheckout to retrieve the associated order.
 *
 *  Note: There's no guarantee that the BUYCheckout returned will be the same as the one that is passed in.
 *  We recommended using the BUYCheckout returned in the block.
 *
 *  @param checkout The BUYCheckout to retrieve completion status for
 *  @param block    (^BUYDataCheckoutStatusBlock)(BUYCheckout *checkout, BUYStatus status, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getCompletionStatusOfCheckout:(BUYCheckout *)checkout completion:(BUYDataCheckoutStatusBlock)block;

#pragma mark - Shipping Rates

/**
 *  Retrieves a list of applicable shipping rates for a given BUYCheckout.
 *  Add the preferred/selected BUYShippingRate to BUYCheckout, then update BUYCheckout
 *
 *  @param checkout The BUYCheckout to retrieve shipping rates for
 *  @param block    (^BUYDataShippingRatesBlock)(NSArray *shippingRates, BUYStatus status, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getShippingRatesForCheckout:(BUYCheckout *)checkout completion:(BUYDataShippingRatesBlock)block;

#pragma mark - Payment Management

/**
 *  Prepares a credit card for usage during the checkout process. This sends it to Shopify's secure servers.
 *  Note: Storing the token does not charge the associated card (credit or otherwise).
 *  The card will be charged upon finalizing the checkout (completeCheckout:completion:)
 *
 *  @param creditCard BUYCreditCard to prepare for usage
 *  @param checkout   The BUYCheckout associated to the purchase.
 *                    The `billingAddress` stored on the BUYCheckout object is optional and recommended and
 *                    used (if provided) to help with fraud checking.
 *  @param block      (^BUYDataCreditCardBlock)(BUYCheckout *checkout, NSString *paymentSessionId, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)storeCreditCard:(id <BUYSerializable>)creditCard checkout:(BUYCheckout *)checkout completion:(BUYDataCreditCardBlock)block;

@end
