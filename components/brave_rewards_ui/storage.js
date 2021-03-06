/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

const debounce = require('../common/debounce')

const keyName = 'rewards-data'

const cleanData = (state) => ({})

module.exports.getInitialState = () => cleanData({})

module.exports.load = () => {
  const data = window.localStorage.getItem(keyName)
  let state
  if (data) {
    try {
      state = JSON.parse(data)
    } catch (e) {
      console.error('Could not parse local storage data: ', e)
    }
  }
  return cleanData(state)
}

module.exports.debouncedSave = debounce((data) => {
  if (data) {
    window.localStorage.setItem(keyName, JSON.stringify(cleanData(data)))
  }
}, 50)
