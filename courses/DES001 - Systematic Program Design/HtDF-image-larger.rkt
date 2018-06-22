;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname HtDF-image-larger) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
; \todo: how to specify "Language\Choose Language ...\Teaching languages\How to design programs - beginning student"
;#lang racket/base
;(require lang/htdp-beginner)
(require 2htdp/image)

(define (image-area image)
  (* (image-width image) (image-height image)))
(check-expect (image-area (circle 15 "solid" "cyan")) 900)
(check-expect (image-area (rectangle 90 10 "solid" "cyan")) 900)

;; Image, Image -> Boolean
;; returns true if the first image is greater (has a greater covering box) than the second
;; greater should be expressed in terms of area, but that is not easy to compute for all the available image types
(check-expect (image-larger? (circle 15 "solid" "cyan") (rectangle 90 10 "solid" "cyan")) false) ; equal areas
(check-expect (image-larger? (circle 20 "solid" "cyan") (rectangle 90 10 "solid" "cyan")) true)
(check-expect (image-larger? (circle 20 "solid" "cyan") (rectangle 17 100 "solid" "cyan")) false)
(check-expect (image-larger? (triangle 14 "solid" "red") (circle 7 "solid" "yellow")) false)
(check-expect (image-larger? (triangle 14 "solid" "red") (circle 6 "solid" "yellow")) true)

;(define (image-larger? image1 image2) false) ;stub

;(define (image-larger? image1 image2) ; template
; (... image1 image2))

(define (image-larger? image1 image2)
  (> (image-area image1) (image-area image2)))