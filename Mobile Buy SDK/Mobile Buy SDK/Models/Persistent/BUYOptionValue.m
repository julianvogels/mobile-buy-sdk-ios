//
//  _BUYOptionValue.m
//  Mobile Buy SDK
//
//  Created by Shopify.
//  Copyright (c) 2015 Shopify Inc. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import "BUYOptionValue.h"

static NSString *const defaultTitleString = @"Default Title";
static NSString *const defaultString = @"Default";
static NSString *const titleString = @"Title";

@implementation BUYOptionValue
@synthesize isDefault;

- (BOOL)isEqualToOptionValue:(BUYOptionValue *)other
{
	return [other isKindOfClass:[self class]] && [self.name isEqual:other.name] && [self.value isEqual:other.value];
}

- (BOOL)isDefault
{
	return [self.name isEqualToString:titleString] &&
	([self.value isEqualToString:defaultTitleString] || [self.value isEqualToString:defaultString]);
}

#if !defined CORE_DATA_PERSISTENCE
- (BOOL)isEqual:(id)object
{
	return [super isEqual:object] || [self isEqualToOptionValue:object];
}

- (NSUInteger)hash
{
	NSUInteger hash = self.name.hash;
	return ((hash << 5) + hash) + self.value.hash;
}
#endif

@end
