# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Code modified by Peiling Jiang
# IMA, 2019

# [START drive_quickstart]

from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload
import auth

target_folder = 'goldfish_photos'


def upload(file_name, file_path, type='image/jpeg'):
    # Upload photos in this project
    photo_metadata = {'name': file_name}
    media = MediaFileUpload(file_path,
                            mimetype=type)
    photo = service.files().create(body=photo_metadata,
                                   media_body=media,
                                   fields='id').execute()
    print('File ID: %s' % photo.get('id'))


if __name__ == '__main__':
    pass
else:
    # auth will search for token.pickle, or create one
    auth.main()

    service = build('drive', 'v3', credentials=auth.creds)

    # Check if the target folder is already in Google Drive
    response = service.files().list(
        q="name='" + target_folder + "' and mimeType='application/vnd.google-apps.folder'",
        spaces='drive'
    ).execute()
    if not response['files']:
        # Create the folder
        file_metadata = {
            'name': target_folder,
            'mimeType': 'application/vnd.google-apps.folder'
        }
        file = service.files().create(body=file_metadata,
                                      fields='id').execute()

# [END drive_quickstart]
