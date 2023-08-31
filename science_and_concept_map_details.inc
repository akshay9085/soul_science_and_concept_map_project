<?php
// soul science and concept map display completed proposals
function science_and_concept_map_completed_proposals_all()
{
	$output = "";
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($soul_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "We welcome your contributions." ."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following Science and Concept Map." . "<hr>";
		$preference_rows = array();
		$i = $result->rowCount();
		while ($row = $result->fetchObject())
		{
			$proposal_id = $row->id;
			$query1 = db_select('soul_science_and_concept_map_submitted_abstracts_file');
			$query1->fields('soul_science_and_concept_map_submitted_abstracts_file');
			$query1->condition('file_approval_status', 1);
			$query1->condition('proposal_id', $proposal_id);
			$soul_project_files = $query1->execute();
			$soul_project_abstract = $soul_project_files->fetchObject();
			$completion_date = date("Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "science-and-concept-map-project/soul-science-and-concept-map-run/" . $row->id),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i--;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No.',
			'Science and Concept Map Project',
			'Contributor Name',
			'University / Institute',
			'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}
// soul science and concept map display in progress proposals
function science_and_concept_map_progress_all()
{
	$page_content = "";
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "We welcome your contributions.<hr>";
	} //$result->rowCount() == 0
	else
	{
		$i = $result->rowCount();
		$page_content .= "Work is in progress for the following Science and Concept Map<hr>";
		$preference_rows = array();
		// $i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i--;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Science and Concept Map Project',
			'Contributor Name',
			'Institute',
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}


function tbc_completed_books_display_new_category_all()
{
    $category_id = NULL;
    $output = "";
    $tbc_books_display_new_category_form = drupal_get_form("tbc_books_display_new_category_form");
    $output .= drupal_render($tbc_books_display_new_category_form);
    return $output;
}
function tbc_books_display_new_category_form($form, &$form_state)
{
    $category_default_value = 0;
        $countresult = db_query("SELECT count(DISTINCT po.id) c
      FROM soul_science_and_concept_map_proposal po
      WHERE po.approval_status = 3 ORDER BY po.id DESC");
    $count_row = $countresult->fetchObject();
    $book_count = $count_row->c;
    $form['completed_book_count'] = array(
        '#type' => 'item',
        '#markup' => "Total number of completed books: " . $book_count . "<br><span style='color:red;'>The list below is not the books as named but only are the solved example for R</span>",
    );
    $form['category'] = array(
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => _list_of_display_category(),
        '#default_value' => $category_default_value,
        '#ajax' => array(
            'callback' => 'ajax_display_all_book_list_callback'
        ),
        '#attributes' => array(
        'style'=>' word-break: break-all;
                    white-space: normal;'
        ),
        '#validated' => TRUE
    );
    $form['book'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="ajax-book-list-replace">',
        '#suffix' => '</div>',
        '#markup' => _list_of_all_completed_books()
    );
    return $form;
}


function _list_of_display_category()
{
    $category[0] = "All";
        $query = db_select('class_table');
        $query->fields('class_table');
        $query->orderBy('id', 'ASC');
        $category_list = $query->execute();
    while ($category_list_data = $category_list->fetchObject())
    {
        $category[$category_list_data->class_name] = $category_list_data->class_name;
    } //$category_list_data = $category_list->fetchObject()
    return $category;
}
function ajax_display_all_book_list_callback($form, $form_state)
{
    $category_default_value = $form_state['values']['category'];
	//var_dump($category_default_value);die;
    if ($category_default_value != '0')
    {
        //$form['subcategory']['#options'] = _list_of_subcategory($category_default_value);
        $form['book']['#markup'] = _list_of_all_completed_books($category_default_value);
        //$commands[] = ajax_command_replace("#ajax-subcategory-list-replace", drupal_render($form['subcategory']));
        $commands[] = ajax_command_replace("#ajax-book-list-replace", drupal_render($form['book']));
    } //$category_default_value > 0
    else
    {
        $form['book']['#markup'] = _list_of_all_completed_books();
        $commands[] = ajax_command_replace("#ajax-book-list-replace", drupal_render($form['book']));
    }
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function _list_of_all_completed_books($class_name = NULL)
{
	//var_dump($class_name);die;			
    $output = "";
    //get the book count
    // $result = db_query("SELECT COUNT( pe.book ) AS book_count FROM {textbook_companion_preference} pe LEFT JOIN textbook_companion_proposal po ON pe.proposal_id = po.id WHERE po.proposal_status =3 AND pe.approval_status =1");
	$result = db_query("SELECT count(po.id) c
	FROM soul_science_and_concept_map_proposal po WHERE po.approval_status = 3 ORDER BY po.id DESC");
    $row = $result->fetchObject();
    $book_count = $row->c;
    $i = $book_count;
    /* get preference */
    if ($class_name == NULL)
    {
        $preference_q = db_query("SELECT po.id,
      po.project_title as project_title, po.approval_date as approval_date, po.actual_completion_date as completion_date, po.contributor_name as full_name, po.university as university 
      FROM soul_science_and_concept_map_proposal po WHERE po.approval_status = 3
    ORDER BY po.id");
    } //$category_default_value <= 0 && $subcategory_default_value == NULL
    else
    {
        $preference_q = db_query("SELECT  po.id,
		po.project_title as project_title, po.approval_date as approval_date, po.actual_completion_date as completion_date, po.contributor_name as full_name, po.university as university 
		FROM soul_science_and_concept_map_proposal po WHERE po.approval_status = 3 AND po.year_of_study= :class_name", array(
            "class_name" => $class_name,
        ));
    }
	//var_dump($preference_q->rowCount());die;
    while ($preference_data = $preference_q->fetchObject())
    {
        $proposal_rows[] = array(
            $i,
            //l($preference_data->project_title, 'textbook_run/' . $preference_data->pe_id . '/' . $preference_data->sub_category . '/' . $preference_data->category_id),
			$preference_data->project_title,
            $preference_data->full_name,
            $preference_data->university,
            date("Y", $preference_data->completion_date)
        );
        $i--;
    } //$proposal_data = $proposal_q->fetchObject()
    /* check if there are any pending proposals */
    if (!$proposal_rows)
    {
        $output .= t('There are no books availabe in this sub category.');
    } //!$proposal_rows
    else
    {
        $proposal_header = array(
            'No.',
            'Title of the Book',
            'Contributor Name',
            'University / Institute',
            'Year of Completion'
        );
        $output .= theme('table', array(
            'header' => $proposal_header,
            'rows' => $proposal_rows
        ));
    } //!$proposal_rows
    //    $output .= "Book count with category: " . $book_count;
    return $output;
}